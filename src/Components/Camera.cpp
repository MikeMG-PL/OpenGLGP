#include "Components/Camera.h"
#include "Engine/GameObject.h"

void Camera::Update()
{
	Component::Update();
	cameraLogic();
}

void Camera::cameraLogic()
{
	target = glm::vec3(0, 0, 0);
	direction = glm::normalize(parent->GetTransform()->position - target);
	right = glm::normalize(glm::cross(worldUp, direction));
	up = glm::cross(direction, right);

	view = glm::lookAt(parent->GetTransform()->position, target, up);
}
