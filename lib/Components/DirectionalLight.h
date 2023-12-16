#pragma once
#include "glm/glm.hpp"
#include "Engine/Shader.h"
#include "Engine/RenderInjector.h"
#include "Helpers/MathHelpers.h"

class DirectionalLight : public RenderInjector
{
public:

	glm::vec3 ambient = { 0.05f, 0.05f, 0.05f };
	glm::vec3 diffuse = { 0.3f, 0.3f, 0.3f };
	glm::vec3 specular = { 0.4f, 0.4f, 0.4f };

	void Start() override;

	void RenderUpdate() override;

private:

	glm::vec3 direction = {};
	std::shared_ptr<GameObject> visualizer1;
	std::shared_ptr<GameObject> visualizer2;
	std::shared_ptr<Shader> shader;
};
