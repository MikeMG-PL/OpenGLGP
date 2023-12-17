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
	shader->setVector3(ambientName.str(), ImVec4ToVec3(s.ambient));
	shader->setVector3(diffuseName.str(), ImVec4ToVec3(s.diffuse));
	shader->setVector3(specularName.str(), ImVec4ToVec3(s.specular));
	shader->setFloat(constantName.str(), s.constant);
	shader->setFloat(linearName.str(), s.linear);
	shader->setFloat(quadraticName.str(), s.quadratic);
	shader->setFloat(cutOffName.str(), glm::cos(glm::radians(s.cutOff)));
	shader->setFloat(outerCutOffName.str(), glm::cos(glm::radians(s.outerCutOff)));

	instancedShader->use();
	instancedShader->setVector3(positionName.str(), position);
	instancedShader->setVector3(directionName.str(), eulerToVector(direction));
	instancedShader->setVector3(ambientName.str(), ImVec4ToVec3(s.ambient));
	instancedShader->setVector3(diffuseName.str(), ImVec4ToVec3(s.diffuse));
	instancedShader->setVector3(specularName.str(), ImVec4ToVec3(s.specular));
	instancedShader->setFloat(constantName.str(), s.constant);
	instancedShader->setFloat(linearName.str(), s.linear);
	instancedShader->setFloat(quadraticName.str(), s.quadratic);
	instancedShader->setFloat(cutOffName.str(), glm::cos(glm::radians(s.cutOff)));
	instancedShader->setFloat(outerCutOffName.str(), glm::cos(glm::radians(s.outerCutOff)));
}
