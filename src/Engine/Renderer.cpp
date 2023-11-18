#include "Engine/Renderer.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <stdio.h>
#include <filesystem>

#include <iostream>
#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION
#include "Components/Camera.h"
#include "Helpers/stb_image.h"
#include "Engine/Editor.h"
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

	// Declarations
	GLfloat vertices[] = {

		// Base
		-0.5f, 0.0f, -0.289f, 0, 0, 0, 0.0f, 1.0f,
		0.5f, 0.0f, -0.289f, 0, 0, 0, 1.0f, 1.0f,
		0.0f, 0.0f, 0.577f, 0, 0, 0, 0.5f, 0.0f,

		// Front
		-0.5f, 0.0f, -0.289f, 0, 0, 0, 0.0f, 1.0f,
		0.5f, 0.0f, -0.289f, 0, 0, 0, 1.0f, 1.0f,
		0.0f, 0.816f, 0.0f, 0, 0, 0, 0.5f, 0.5f,

		// Right
		0.5f, 0.0f, -0.289f, 0, 0, 0, 1.0f, 1.0f,
		0.0f, 0.0f, 0.577f, 0, 0, 0, 0.5f, 0.0f,
		0.0f, 0.816f, 0.0f, 0, 0, 0, 0.5f, 0.5f,

		// Left
		0.0f, 0.0f, 0.577f, 0, 0, 0, 0.5f, 0.0f,
		-0.5f, 0.0f, -0.289f, 0, 0, 0, 0.0f, 1.0f,
		0.0f, 0.816f, 0.0f, 0, 0, 0, 0.5f, 0.5f,
	};

	shader = Shader(0, vertexShaderPath, fragmentShaderPath);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("../../res/textures/cow.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;

	stbi_image_free(data);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// TexCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader.use();

	InitUniformLocs();

	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, zoom));
	projection = glm::perspective(glm::radians(45.0f), static_cast<float>(windowX) / static_cast<float>(windowY), 0.1f, 100.0f);

	/////////////////////////////////////////////////////////////////////////

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

void Renderer::Render(const Camera& camera)
{
	int display_w, display_h;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	SetBackgroundColor(Editor::Get().GetBackgroundColor());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);

	const ImVec4 drawingColor = Editor::Get().GetDrawingColor();
	glUniform4f(customColorLoc, drawingColor.x, drawingColor.y, drawingColor.z, drawingColor.w);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
	view = camera.view;
	// view = glm::translate(view, glm::vec3(0.0f, 0.0f, zoom));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	DrawRecursiveSierpinski(shader, 1, recursionLevels, model);
}

void Renderer::Unbind()
{
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::Cleanup()
{
	shader.cleanup();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Renderer::SetBackgroundColor(ImVec4 color)
{
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawRecursiveSierpinski(const Shader& shader, int const depth, int const maxDepth, glm::mat4 model)
{
	if (depth == maxDepth)
	{
		model = glm::rotate(model, 45.0f, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(0, 0.272f / 2.0f, 0));
		float const scale = 1.0f / glm::pow(2.0f, static_cast<float>(depth) - 1.0f);
		shader.setMat4("model", glm::scale(model, glm::vec3(scale, scale, scale)));
		glDrawArrays(GL_TRIANGLES, 0, 12);
		return;
	}

	int const next_depth = depth + 1;
	float const translation = 1.0f / glm::pow(2.0f, next_depth);

	DrawRecursiveSierpinski(shader, next_depth, maxDepth, glm::translate(model, glm::vec3(0.0f, translation * 0.816f, 0.0)));
	DrawRecursiveSierpinski(shader, next_depth, maxDepth, glm::translate(model, glm::vec3(-translation, -translation * 0.816f, translation * 0.6f)));
	DrawRecursiveSierpinski(shader, next_depth, maxDepth, glm::translate(model, glm::vec3(translation, -translation * 0.816f, translation * 0.6f)));
	DrawRecursiveSierpinski(shader, next_depth, maxDepth, glm::translate(model, glm::vec3(0.0f, -translation * 0.816f, -translation * 1.15f)));
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
