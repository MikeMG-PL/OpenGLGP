#pragma once
#include "Engine/RenderInjector.h"
#include "Engine/Shader.h"

class SpotLight : public RenderInjector, public std::enable_shared_from_this<SpotLight>
{
public:

	void Start() override;
	void RenderUpdate() override;

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
