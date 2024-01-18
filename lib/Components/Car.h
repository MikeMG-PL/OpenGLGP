#pragma once
#include "Transform.h"
#include "Engine/Component.h"
#include "GLFW/glfw3.h"

class Car : public Component
{
public:

	void Start() override;
	void Update() override;

	float maxSpeed = 15;
	glm::vec3 cameraOffset = { 0, -7, 1.75f };

	std::shared_ptr<Transform> leftFront;
	std::shared_ptr<Transform> rightFront;
	std::shared_ptr<Transform> leftBack;
	std::shared_ptr<Transform> rightBack;

	std::shared_ptr<Transform> leftDoor;
	std::shared_ptr<Transform> rightDoor;

private:

	void processInput();
	void steering();

	float currentSpeed = 0;
	std::shared_ptr<Transform> transform;
	GLFWwindow* window;

	bool enterKeyWasPressed = false;
};
