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

glm::vec3 Camera::GetUp() const
{
	return up;
}

glm::vec3 Camera::GetFront() const
{
	return direction;
}

void Camera::cameraLogic()
{
	direction = eulerToDirectionVector(parent->GetTransform()->localEulerAngles);
	target = parent->GetTransform()->localPosition + direction;
	right = glm::normalize(glm::cross(worldUp, direction));
	up = glm::cross(direction, right);

	auto v = eulerToDirectionVector(parent->GetTransform()->localEulerAngles);
	std::cout << v.x << ", " << v.y << ", " << v.z << std::endl;

	view = glm::lookAt(parent->GetTransform()->localPosition, target, up);
}

glm::vec3 Camera::eulerToDirectionVector(glm::vec3 euler)
{
	glm::vec3 v = { euler.x, euler.y - 90, euler.z };

	float cosPitch = glm::cos(glm::radians(v.x));
	float sinPitch = glm::sin(glm::radians(v.x));
	float cosYaw = glm::cos(glm::radians(v.y));
	float sinYaw = glm::sin(glm::radians(v.y));

	v = { cosPitch * cosYaw, sinPitch, cosPitch * sinYaw };

	return v;
}