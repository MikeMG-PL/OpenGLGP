#pragma once
#include "glm/glm.hpp"
#include "Engine/RenderInjector.h"

class DirectionalLight : public RenderInjector
{
public:

	glm::vec3 ambient = { 0.05f, 0.05f, 0.05f };
	glm::vec3 diffuse = { 0.4f, 0.4f, 0.4f };
	glm::vec3 specular = { 0.5f, 0.5f, 0.5f };

	void Start() override;

	void RenderUpdate() override;

private:

	glm::vec3 eulerToVector(const glm::vec3 euler);
	glm::vec3 direction = {};
	std::shared_ptr<GameObject> visualizer1;
	std::shared_ptr<GameObject> visualizer2;
};
