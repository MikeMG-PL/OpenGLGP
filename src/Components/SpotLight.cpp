#include "Components/SpotLight.h"

#include "Components/Model.h"
#include "Engine/GameObject.h"
#include "Engine/Renderer.h"
#include "Helpers/MathHelpers.h"

void SpotLight::Start()
{
	RenderInjector::Start();

	shader = std::make_shared<Shader>(Renderer::Get().GetShader());
	instancedShader = std::make_shared<Shader>(Renderer::Get().GetInstancedShader());
	Renderer::Get().RegisterSpotLight(shared_from_this());
	ID = Renderer::Get().GetSpotLights()->size() - 1;

	visualizer1 = GameObject::CreateObject();

	visualizer1->AddComponent<Model>("../../res/models/visualisers/roof.dae", glm::vec2(1, 1), true);

	GetParent()->GetTransform()->AddChild(visualizer1->GetTransform());
	visualizer1->GetTransform()->localScale = { 0.1f, 0.1f, 0.5f };

	uniform << "spotLights[" << ID << "].";
	positionName << uniform.str() << "position";
	directionName << uniform.str() << "direction";
	ambientName << uniform.str() << "ambient";
	diffuseName << uniform.str() << "diffuse";
	specularName << uniform.str() << "specular";
	constantName << uniform.str() << "constant";
	linearName << uniform.str() << "linear";
	quadraticName << uniform.str() << "quadratic";
	cutOffName << uniform.str() << "cutOff";
	outerCutOffName << uniform.str() << "outerCutOff";
}

void SpotLight::RenderUpdate()
{
	RenderInjector::RenderUpdate();

	position = GetParent()->GetTransform()->GetPosParentSpace();
	direction = GetParent()->GetTransform()->GetRotParentSpace();

	shader->use();
	shader->setVector3(positionName.str(), position);
	shader->setVector3(directionName.str(), eulerToVector(direction));
	shader->setVector3(ambientName.str(), ambient);
	shader->setVector3(diffuseName.str(), diffuse);
	shader->setVector3(specularName.str(), specular);
	shader->setFloat(constantName.str(), constant);
	shader->setFloat(linearName.str(), linear);
	shader->setFloat(quadraticName.str(), quadratic);
	shader->setFloat(cutOffName.str(), glm::cos(glm::radians(cutOff)));
	shader->setFloat(outerCutOffName.str(), glm::cos(glm::radians(outerCutOff)));

	instancedShader->use();
	instancedShader->setVector3(positionName.str(), position);
	instancedShader->setVector3(directionName.str(), eulerToVector(direction));
	instancedShader->setVector3(ambientName.str(), ambient);
	instancedShader->setVector3(diffuseName.str(), diffuse);
	instancedShader->setVector3(specularName.str(), specular);
	instancedShader->setFloat(constantName.str(), constant);
	instancedShader->setFloat(linearName.str(), linear);
	instancedShader->setFloat(quadraticName.str(), quadratic);
	instancedShader->setFloat(cutOffName.str(), glm::cos(glm::radians(cutOff)));
	instancedShader->setFloat(outerCutOffName.str(), glm::cos(glm::radians(outerCutOff)));
}
