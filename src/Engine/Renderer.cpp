#include "Engine/Renderer.h"
#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "Animation system/AnimXForm.h"
#include <stdio.h>
#include <filesystem>
#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION
#include "Components/Camera.h"
#include "Helpers/aiHelpers.h"
#include "Components/Model.h"
#include "Helpers/stb_image.h"
#include "Engine/Editor.h"
#include "Engine/GameInstance.h"
#include "Engine/GameObject.h"
#include "Engine/RenderInjector.h"
#include "Engine/Shader.h"

#include <windows.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Animation system/RiggedModel.h"

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

Renderer& Renderer::Get()
{
	static Renderer instance;
	return instance;
}

bool Renderer::Init(int X, int Y)
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return false;

	windowX = X;
	windowY = Y;

	// Decide GL+GLSL versions
	glsl_version = "#version 430";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

	// Create window with graphics context
	window = glfwCreateWindow(X, Y, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if (window == NULL)
		return false;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	if (bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		spdlog::error("Failed to initialize OpenGL loader!");
		return false;
	}
	spdlog::info("Successfully initialized OpenGL loader!");

	Editor::Get().Init();
	glEnable(GL_DEPTH_TEST);
	shader = Shader(0, vertexShaderPath, fragmentShaderPath);
	instancedShader = Shader(1, instancedVertexShaderPath, fragmentShaderPath);
	cubemapShader = Shader(2, cubemapVertexShaderPath, cubemapFragmentShaderPath);
	reflectionShader = Shader(3, vertexShaderPath, reflectionFragmentShaderPath);
	refractionShader = Shader(4, vertexShaderPath, refractionFragmentShaderPath);
	InitUniformLocs();
	cubemap = SetupCubemap();

	return true;
}

glm::vec2 Renderer::GetWindowSize() const
{
	return { windowX, windowY };
}

GLFWwindow* Renderer::GetWindow() const
{
	return window;
}

const char* Renderer::GetGLSLVersion() const
{
	return glsl_version;
}

Shader Renderer::GetShader() const
{
	return shader;
}

Shader Renderer::GetInstancedShader() const
{
	return instancedShader;
}

Shader Renderer::GetReflectionShader() const
{
	return reflectionShader;
}

glm::mat4 Renderer::GetModelMatrix() const
{
	return model;
}

glm::mat4 Renderer::GetViewMatrix() const
{
	return view;
}

glm::mat4 Renderer::GetProjectionMatrix() const
{
	return projection;
}

void Renderer::Render(const Camera& camera)
{
	int display_w, display_h;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	SetBackgroundColor(Editor::Get().GetBackgroundColor());

	const ImVec4 drawingColor = Editor::Get().GetDrawingColor();
	glUniform4f(customColorLoc, drawingColor.x, drawingColor.y, drawingColor.z, drawingColor.w);

	shader.use();

	view = camera.view;
	projection = glm::perspective(glm::radians(45.0f), static_cast<float>(windowX) / static_cast<float>(windowY), 0.1f, 1000.0f);

	shader.setVector3("viewPos", camera.GetParent()->GetTransform()->localPosition);
	shader.setInt("numPointLights", GetPointLights()->size());
	shader.setInt("numSpotLights", GetSpotLights()->size());
	instancedShader.use();
	instancedShader.setVector3("viewPos", camera.GetParent()->GetTransform()->localPosition);
	instancedShader.setInt("numPointLights", GetPointLights()->size());
	instancedShader.setInt("numSpotLights", GetSpotLights()->size());

	auto allGameObjects = GameInstance::Get().allGameObjects;

	for (const auto& gameObjectPtr : allGameObjects)
	{
		if (gameObjectPtr->allowUpdate)
			gameObjectPtr->GetTransform()->UpdateSelfAndChild();

		if (const auto injector = gameObjectPtr->GetComponent<RenderInjector>())
			injector->RenderUpdate();

		if (const auto riggedModelComponent = gameObjectPtr->GetComponent<RiggedModel>())
		{
			model = gameObjectPtr->GetTransform()->modelMatrix;
			shader.use();
			shader.setMat4("projection", projection);
			shader.setMat4("view", view);
			shader.setMat4("model", model);

			riggedModelComponent->Draw(shader);
		}

		if (const auto modelComponent = gameObjectPtr->GetComponent<Model>())
		{
			// Render non-reflective models
			if (!modelComponent->IsInstanced())
			{
				switch (modelComponent->GetMaterial())
				{
				default:
					model = gameObjectPtr->GetTransform()->modelMatrix;
					shader.use();
					shader.setMat4("projection", projection);
					shader.setMat4("view", view);
					shader.setMat4("model", model);
					modelComponent->Draw(shader);
					break;

				case REFLECTIVE:
					model = gameObjectPtr->GetTransform()->modelMatrix;
					reflectionShader.use();
					reflectionShader.setMat4("projection", projection);
					reflectionShader.setMat4("view", view);
					reflectionShader.setMat4("model", model);
					reflectionShader.setVector3("viewPos", camera.GetParent()->GetTransform()->localPosition);
					reflectionShader.setFloat("dimValue", modelComponent->dimValue);
					modelComponent->Draw(reflectionShader);
					break;

				case REFRACTIVE:
					model = gameObjectPtr->GetTransform()->modelMatrix;
					refractionShader.use();
					refractionShader.setMat4("projection", projection);
					refractionShader.setMat4("view", view);
					refractionShader.setMat4("model", model);
					refractionShader.setVector3("viewPos", camera.GetParent()->GetTransform()->localPosition);
					refractionShader.setFloat("dimValue", modelComponent->dimValue);
					modelComponent->Draw(refractionShader);
					break;
				}
			}
		}
	}

	DrawCubemap(camera);
}

void Renderer::Cleanup()
{
	shader.cleanup();
	instancedShader.cleanup();
	cubemapShader.cleanup();
}

void Renderer::SetBackgroundColor(ImVec4 color)
{
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::RegisterPointLight(const std::shared_ptr<PointLight>& pointLight)
{
	pointLights.emplace_back(pointLight);
}

const std::vector<std::shared_ptr<PointLight>>* Renderer::GetPointLights() const
{
	return &pointLights;
}

void Renderer::RegisterSpotLight(const std::shared_ptr<SpotLight>& spotLight)
{
	spotLights.emplace_back(spotLight);
}

const std::vector<std::shared_ptr<SpotLight>>* Renderer::GetSpotLights() const
{
	return &spotLights;
}

bool Renderer::ShouldCloseWindow()
{
	return glfwWindowShouldClose(window);
}

void Renderer::CloseWindow()
{
	glfwSetWindowShouldClose(window, true);
}

void Renderer::glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

unsigned Renderer::SetupCubemap()
{
	stbi_set_flip_vertically_on_load(false);
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < texturesFaces.size(); i++)
	{
		unsigned char* data = stbi_load(texturesFaces[i].c_str(), &width, &height, &nrChannels, 3);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << texturesFaces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	stbi_set_flip_vertically_on_load(true);

	return textureID;
}

void Renderer::DrawCubemap(const Camera& camera)
{
	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	cubemapShader.use();
	view = glm::mat4(glm::mat3(camera.view)); // remove translation from the view matrix
	cubemapShader.setMat4("view", view);
	cubemapShader.setMat4("projection", projection);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void Renderer::InitUniformLocs()
{
	modelLoc = glGetUniformLocation(shader.ID, "model");
	viewLoc = glGetUniformLocation(shader.ID, "view");
	projectionLoc = glGetUniformLocation(shader.ID, "projection");
	customColorLoc = glGetUniformLocation(shader.ID, "customColor");
}
