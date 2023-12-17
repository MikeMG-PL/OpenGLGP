#include "Components/DirectionalLight.h"

#include <glm/ext/matrix_transform.hpp>

#include "Components/Model.h"
#include "Engine/Editor.h"
#include "Engine/GameObject.h"
#include "Engine/Renderer.h"

void DirectionalLight::Start()
{
	RenderInjector::Start();

	shader = std::make_shared<Shader>(Renderer::Get().GetShader());
	instancedShader = std::make_shared<Shader>(Renderer::Get().GetInstancedShader());

	visualizer1 = GameObject::CreateObject();
	visualizer2 = GameObject::CreateObject();

	visualizer1->AddComponent<Model>("../../res/models/visualisers/roof.dae", glm::vec2(1, 1), true);
	visualizer2->AddComponent<Model>("../../res/models/visualisers/hut.dae", glm::vec2(1, 1), true);

	GetParent()->GetTransform()->AddChild(visualizer1->GetTransform());
	GetParent()->GetTransform()->AddChild(visualizer2->GetTransform());

	visualizer1->GetTransform()->localScale = { 0.1f, 0.1f, 0.5f };
	visualizer1->GetTransform()->localEulerAngles.x += 180;

	visualizer2->GetTransform()->localScale = { 0.03f, 0.03f, 0.2f };
	visualizer2->GetTransform()->localPosition = { 0, 0, 2 };
}

void DirectionalLight::RenderUpdate()
{
	RenderInjector::RenderUpdate();

	direction = GetParent()->GetTransform()->GetRotParentSpace();

	shader->use();
	shader->setVector3("dirLight.direction", eulerToVector(direction));
	shader->setVector3("dirLight.ambient", ImVec4ToVec3(d.ambient));
	shader->setVector3("dirLight.diffuse", ImVec4ToVec3(d.diffuse));
	shader->setVector3("dirLight.specular", ImVec4ToVec3(d.specular));

	instancedShader->use();
	instancedShader->setVector3("dirLight.direction", eulerToVector(direction));
	instancedShader->setVector3("dirLight.ambient", ImVec4ToVec3(d.ambient));
	instancedShader->setVector3("dirLight.diffuse", ImVec4ToVec3(d.diffuse));
	instancedShader->setVector3("dirLight.specular", ImVec4ToVec3(d.specular));

}
