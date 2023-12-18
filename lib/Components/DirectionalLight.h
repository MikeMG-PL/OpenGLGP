#pragma once
#include "glm/glm.hpp"
#include "Engine/Shader.h"
#include "Engine/RenderInjector.h"
#include "Helpers/MathHelpers.h"

struct DirectionalLightParams
{
	ImVec4 ambient = { 0.05f, 0.05f, 0.05f, 1 };
	ImVec4 diffuse = { 0.3f, 0.3f, 0.3f, 1 };
	ImVec4 specular = { 0.4f, 0.4f, 0.4f, 1 };
	bool enabled = true;
};

class DirectionalLight : public RenderInjector
{
public:

	DirectionalLightParams d;

	void Start() override;

	void RenderUpdate() override;

private:

	glm::vec3 direction = {};
	std::shared_ptr<GameObject> visualizer1;
	std::shared_ptr<GameObject> visualizer2;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Shader> instancedShader;
};
