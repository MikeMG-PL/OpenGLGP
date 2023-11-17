#pragma once
#include "glm/glm.hpp"
#include "Shader.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLAD

// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually. 
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

class ImVec4;
struct GLFWwindow;

class Renderer
{
public:

	Renderer& operator=(const Renderer&) = delete;
	Renderer(const Renderer&) = delete;
	static Renderer& Get();

	bool Init(int X, int Y);
	void Render();
	void Unbind();
	void Cleanup();
	glm::vec2 GetWindowSize() const;
	GLFWwindow* GetWindow() const;
	const char* GetGLSLVersion() const;
	void SetBackgroundColor(ImVec4 color);
	void DrawRecursiveSierpinski(const Shader& shader, int const depth, int const maxDepth, glm::mat4 model);

	bool ShouldCloseWindow();
	void CloseWindow();
	static void glfw_error_callback(int error, const char* description);

private:

	int windowX = 0, windowY = 0;
	GLFWwindow* window;
	const char* glsl_version;
	const GLchar* vertexShaderPath = "../../lib/shaders/vertexShader.vert";
	const GLchar* fragmentShaderPath = "../../lib/shaders/fragmentShader.frag";

	/// Before decoupling to proper components ///

	GLuint texture;
	Shader shader;
	GLuint VAO, VBO, EBO;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	float rotationX = 0;
	float rotationY = 0;
	int recursionLevels = 1;
	float zoom = -3.0f;

	GLuint modelLoc;
	GLuint viewLoc;
	GLuint projectionLoc;
	GLint customColorLoc;

	void InitUniformLocs();

	////

	Renderer() = default;
};
