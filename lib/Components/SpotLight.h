#pragma once
#include "Engine/RenderInjector.h"
#include "Engine/Shader.h"

class SpotLight : public RenderInjector, public std::enable_shared_from_this<SpotLight>
{
public:

	void Start() override;
	void RenderUpdate() override;

	glm::vec3 ambient = { 0.0f, 0.0f, 0.0f };
	glm::vec3 diffuse = { 1.0f, 1.0f, 1.0f };
	glm::vec3 specular = { 1.0f, 1.0f, 1.0f };
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
	float cutOff = 12.5f;			// in degrees
	float outerCutOff = 15.0f;		// in degrees

private:

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Shader> instancedShader;
	int ID = -1;

	std::stringstream uniform;
	std::stringstream positionName;
	std::stringstream directionName;
	std::stringstream ambientName;
	std::stringstream diffuseName;
	std::stringstream specularName;
	std::stringstream constantName;
	std::stringstream linearName;
	std::stringstream quadraticName;
	std::stringstream cutOffName;
	std::stringstream outerCutOffName;

	glm::vec3 position = {};
	glm::vec3 direction = {};
	std::shared_ptr<GameObject> visualizer1;
};
