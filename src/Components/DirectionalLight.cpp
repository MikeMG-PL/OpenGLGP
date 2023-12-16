#include "Components/DirectionalLight.h"

#include <glm/ext/matrix_transform.hpp>

#include "Components/Model.h"
#include "Engine/GameObject.h"
#include "Engine/Renderer.h"

void DirectionalLight::Start()
{
	RenderInjector::Start();

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

	direction = GetParent()->GetTransform()->localEulerAngles;

	Renderer::Get().GetShader().setVector3("dirLight.direction", eulerToVector(direction));
	Renderer::Get().GetShader().setVector3("dirLight.ambient", ambient);
	Renderer::Get().GetShader().setVector3("dirLight.diffuse", diffuse);
	Renderer::Get().GetShader().setVector3("dirLight.specular", specular);
}

glm::vec3 DirectionalLight::eulerToVector(const glm::vec3& euler)
{
	// Convert degrees to radians
	glm::vec3 eulerRad = glm::radians(euler);

	// Create rotation matrices for pitch, yaw, and roll
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), eulerRad.y, glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), eulerRad.x, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), eulerRad.z, glm::vec3(0.0f, 0.0f, 1.0f));

	// Directional vector (assuming the default direction is along -Z)
	glm::vec4 directionalVector = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

	// Rotate the directional vector using the combined rotation matrix
	directionalVector = rotation * directionalVector;

	// Extract the resulting vector from the rotated directional vector
	return glm::vec3(directionalVector);
}
