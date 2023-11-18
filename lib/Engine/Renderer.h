#pragma once
#include "glm/glm.hpp"
#include <glad/glad.h>
#include "Shader.h"

class Camera;
class ImVec4;
struct GLFWwindow;

class Renderer
{
public:

	Renderer& operator=(const Renderer&) = delete;
	Renderer(const Renderer&) = delete;
	static Renderer& Get();

	bool Init(int X, int Y);
	void Render(const Camera& camera);
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
	const GLchar* vertexShaderPath = "../../lib/Shaders/vertexShader.vert";
	const GLchar* fragmentShaderPath = "../../lib/Shaders/fragmentShader.frag";

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

	//////////////////////////////////////////

	Renderer() = default;
};
