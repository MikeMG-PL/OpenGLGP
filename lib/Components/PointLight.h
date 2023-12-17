#pragma once
#include <imgui.h>

#include "Engine/Shader.h"
#include "Engine/RenderInjector.h"

struct PointLightParams
{
	ImVec4 ambient = { 0.05f, 0.05f, 0.05f, 1 };
	ImVec4 diffuse = { 0.8f, 0.8f, 0.8f, 1 };
	ImVec4 specular = { 1.0f, 1.0f, 1.0f, 1 };
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};

class PointLight : public RenderInjector, public std::enable_shared_from_this<PointLight>
{
public:

	void Start() override;
	void RenderUpdate() override;

	PointLightParams p;

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
