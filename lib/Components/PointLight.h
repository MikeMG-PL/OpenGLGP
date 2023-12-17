#pragma once
#include "Engine/Shader.h"
#include "Engine/RenderInjector.h"
class PointLight : public RenderInjector, public std::enable_shared_from_this<PointLight>
{
public:

	void Start() override;
	void RenderUpdate() override;

	glm::vec3 ambient = { 0.05f, 0.05f, 0.05f };
	glm::vec3 diffuse = { 0.8f, 0.8f, 0.8f };
	glm::vec3 specular = { 1.0f, 1.0f, 1.0f };
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

private:

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Shader> instancedShader;
	int ID = -1;

	std::stringstream uniform;
	std::stringstream positionName;
	std::stringstream ambientName;
	std::stringstream diffuseName;
	std::stringstream specularName;
	std::stringstream constantName;
	std::stringstream linearName;
	std::stringstream quadraticName;

	std::shared_ptr<GameObject> visualizer1;
};
