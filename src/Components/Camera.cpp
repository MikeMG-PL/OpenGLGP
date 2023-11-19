#include "Components/Camera.h"
#include "Engine/GameObject.h"

void Camera::Update()
{
	Component::Update();
	cameraLogic();
}

glm::vec3 Camera::GetWorldUp() const
{
	return worldUp;
}

glm::vec3 Camera::GetWorldFront() const
{
	return worldFront;
}

void Camera::cameraLogic()
{
	target = parent->GetTransform()->localPosition + worldFront;
	direction = glm::normalize(parent->GetTransform()->localPosition - target);
	right = glm::normalize(glm::cross(worldUp, direction));
	up = glm::cross(direction, right);

	view = glm::lookAt(parent->GetTransform()->localPosition, target, up);
}
