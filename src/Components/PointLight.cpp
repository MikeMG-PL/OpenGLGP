#include "Components/PointLight.h"

#include "Components/Model.h"
#include "Engine/GameObject.h"
#include "Engine/Renderer.h"

void PointLight::Start()
{
	RenderInjector::Start();

	shader = std::make_shared<Shader>(Renderer::Get().GetShader());
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

	shader->setVector3(positionName.str() , GetParent()->GetTransform()->localPosition);
	shader->setVector3(ambientName.str(), { 0.05f, 0.05f, 0.05f });
	shader->setVector3(diffuseName.str(), { 0.8f, 0.8f, 0.8f });
	shader->setVector3(specularName.str(), { 1.0f, 1.0f, 1.0f });
	shader->setFloat(constantName.str(), 1.0f);
	shader->setFloat(linearName.str(), 0.09f);
	shader->setFloat(quadraticName.str(), 0.032f);
}
