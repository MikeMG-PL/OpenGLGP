#pragma once
#include <vector>

#include "glm/glm.hpp"
#include <glad/glad.h>
#include "Shader.h"
#include "Components/PointLight.h"
#include "Components/SpotLight.h"

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
	//void Unbind();
	void Cleanup();
	glm::vec2 GetWindowSize() const;
	GLFWwindow* GetWindow() const;
	const char* GetGLSLVersion() const;
	Shader GetShader() const;
	Shader GetInstancedShader() const;
	Shader GetReflectionShader() const;
	glm::mat4 GetModelMatrix() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	void SetBackgroundColor(ImVec4 color);

	void RegisterPointLight(const std::shared_ptr<PointLight>& pointLight);
	const std::vector<std::shared_ptr<PointLight>>* GetPointLights() const;

	void RegisterSpotLight(const std::shared_ptr<SpotLight>& spotLight);
	const std::vector<std::shared_ptr<SpotLight>>* GetSpotLights() const;

	bool ShouldCloseWindow();
	void CloseWindow();
	static void glfw_error_callback(int error, const char* description);

private:

	// Cubemap data
	unsigned SetupCubemap();
	unsigned skyboxVAO, skyboxVBO;
	void DrawCubemap(const Camera& camera);
	unsigned int cubemap;

	std::vector<std::string> texturesFaces =
	{
		"../../res/textures/sky/posx.jpg",
		"../../res/textures/sky/negx.jpg",
		"../../res/textures/sky/posy.jpg",
		"../../res/textures/sky/negy.jpg",
		"../../res/textures/sky/posz.jpg",
		"../../res/textures/sky/negz.jpg"
	};

	float skyboxVertices[108] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// Well, end of cubemap data

	Shader shader;
	Shader instancedShader;
	Shader cubemapShader;
	Shader reflectionShader;
	int windowX = 0, windowY = 0;
	GLFWwindow* window;
	const char* glsl_version;
	const GLchar* vertexShaderPath = "../../lib/Shaders/vertexShader.vert";
	const GLchar* instancedVertexShaderPath = "../../lib/Shaders/instancedVertexShader.vert";
	const GLchar* fragmentShaderPath = "../../lib/Shaders/fragmentShader.frag";
	const GLchar* cubemapVertexShaderPath = "../../lib/Shaders/cubemapVertexShader.vert";
	const GLchar* cubemapFragmentShaderPath = "../../lib/Shaders/cubemapFragmentShader.frag";
	const GLchar* reflectionFragmentShaderPath = "../../lib/Shaders/reflectionFragmentShader.frag";

	std::vector<std::shared_ptr<PointLight>> pointLights;
	std::vector<std::shared_ptr<SpotLight>> spotLights;

	GLuint modelLoc;
	GLuint viewLoc;
	GLuint projectionLoc;
	GLint customColorLoc;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	void InitUniformLocs();

	//////////////////////////////////////////

	Renderer() = default;
};
