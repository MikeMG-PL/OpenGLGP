#include "Components/PointLight.h"

#include "Components/Model.h"
#include "Engine/GameObject.h"
#include "Engine/Renderer.h"

void PointLight::Start()
{
	RenderInjector::Start();

	shader = std::make_shared<Shader>(Renderer::Get().GetShader());
	instancedShader = std::make_shared<Shader>(Renderer::Get().GetInstancedShader());
	Renderer::Get().RegisterPointLight(shared_from_this());
	ID = Renderer::Get().GetPointLights()->size() - 1;

	uniform << "pointLights[" << ID << "].";
	positionName << uniform.str() << "position";
	ambientName << uniform.str() << "ambient";
	diffuseName << uniform.str() << "diffuse";
	specularName << uniform.str() << "specular";
	constantName << uniform.str() << "constant";
	linearName << uniform.str() << "linear";
	quadraticName << uniform.str() << "quadratic";

	visualizer1 = GameObject::CreateObject();
	visualizer1->AddComponent<Model>("../../res/models/sun/sun.obj", glm::vec2(1, 1), true);
	GetParent()->GetTransform()->AddChild(visualizer1->GetTransform());
}

void PointLight::RenderUpdate()
{
	RenderInjector::RenderUpdate();

	shader->use();
	shader->setVector3(positionName.str(), GetParent()->GetTransform()->GetPosParentSpace());
	shader->setVector3(ambientName.str(), ambient);
	shader->setVector3(diffuseName.str(), diffuse);
	shader->setVector3(specularName.str(), specular);
	shader->setFloat(constantName.str(), constant);
	shader->setFloat(linearName.str(), linear);
	shader->setFloat(quadraticName.str(), quadratic);

	instancedShader->use();
	instancedShader->setVector3(positionName.str(), GetParent()->GetTransform()->GetPosParentSpace());
	instancedShader->setVector3(ambientName.str(), ambient);
	instancedShader->setVector3(diffuseName.str(), diffuse);
	instancedShader->setVector3(specularName.str(), specular);
	instancedShader->setFloat(constantName.str(), constant);
	instancedShader->setFloat(linearName.str(), linear);
	instancedShader->setFloat(quadraticName.str(), quadratic);
}
