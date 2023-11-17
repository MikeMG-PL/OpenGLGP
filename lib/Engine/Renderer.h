#pragma once
#include "glm/glm.hpp"

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

class Shader;
class ImVec4;

class Renderer
{
public:

	Renderer& operator=(const Renderer&) = delete;
	Renderer(const Renderer&) = delete;
	//~Renderer();
	static Renderer& Get();

	void Prepare();
	void Render();
	void Unbind();
	void Cleanup();
	void SetBackgroundColor(ImVec4 color);
	void DrawRecursiveSierpinski(const Shader& shader, int const depth, int const maxDepth, glm::mat4 model);

private:

	const GLchar* vertexShaderPath = "../../lib/shaders/vertexShader.vert";
	const GLchar* fragmentShaderPath = "../../lib/shaders/fragmentShader.frag";
	Renderer() = default;

};
