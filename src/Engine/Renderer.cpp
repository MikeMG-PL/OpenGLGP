#include "Engine/Renderer.h"
#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <stdio.h>
#include <filesystem>

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION
#include "Components/Camera.h"
#include "Components/HutSpawner.h"
#include "Components/Model.h"
#include "Helpers/stb_image.h"
#include "Engine/Editor.h"
#include "Engine/GameInstance.h"
#include "Engine/GameObject.h"
#include "Engine/Shader.h"

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
	InitUniformLocs();
	projection = glm::perspective(glm::radians(45.0f), static_cast<float>(windowX) / static_cast<float>(windowY), 0.1f, 1000.0f);
	shader.use();
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

void Renderer::Render(const Camera& camera)
{
	int display_w, display_h;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	SetBackgroundColor(Editor::Get().GetBackgroundColor());

	const ImVec4 drawingColor = Editor::Get().GetDrawingColor();
	glUniform4f(customColorLoc, drawingColor.x, drawingColor.y, drawingColor.z, drawingColor.w);

	view = camera.view;
	shader.setVector3("viewPos", camera.GetParent()->GetTransform()->localPosition);

	auto allGameObjects = GameInstance::Get().allGameObjects;

	for (const auto& gameObjectPtr : allGameObjects)
	{
		if(gameObjectPtr->allowUpdate)
			gameObjectPtr->GetTransform()->UpdateSelfAndChild();

		shader.setVector3("dirLight.direction", { -0.2f, -1.0f, -0.3f });
		shader.setVector3("dirLight.ambient", { 0.05f, 0.05f, 0.05f });
		shader.setVector3("dirLight.diffuse", { 0.4f, 0.4f, 0.4f });
		shader.setVector3("dirLight.specular", { 0.5f, 0.5f, 0.5f });

		if (const auto hutComp = gameObjectPtr->GetComponent<HutSpawner>())
		{
			instancedShader.use();
			instancedShader.setMat4("projection", projection);
			instancedShader.setMat4("view", view);
			instancedShader.setVector3("viewPos", camera.GetParent()->GetTransform()->localPosition);

			hutComp->Draw(instancedShader, 0); // this 0 is for historical reasons
		}
		if (const auto modelComponent = gameObjectPtr->GetComponent<Model>())
		{
			if (!modelComponent->IsInstanced())
			{
				model = gameObjectPtr->GetTransform()->modelMatrix;
				shader.use();
				shader.setMat4("projection", projection);
				shader.setMat4("view", view);
				shader.setMat4("model", model);
				modelComponent->Draw(shader);
			}
			else
				instancedShader.use();
		}
	}
}

void Renderer::Cleanup()
{
	shader.cleanup();
	instancedShader.cleanup();
}

void Renderer::SetBackgroundColor(ImVec4 color)
{
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);
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

void Renderer::InitUniformLocs()
{
	modelLoc = glGetUniformLocation(shader.ID, "model");
	viewLoc = glGetUniformLocation(shader.ID, "view");
	projectionLoc = glGetUniformLocation(shader.ID, "projection");
	customColorLoc = glGetUniformLocation(shader.ID, "customColor");
}
