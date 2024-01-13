#pragma once
#include "Transform.h"
#include "Engine/Component.h"
#include "GLFW/glfw3.h"

class Car : public Component
{
public:

	void Start() override;
	void Update() override;

	float speed = 50;

	glm::vec3 cameraOffset = { 0, -7, 1.75f };

private:

	void processInput();
	void steering();

	float currentSpeed = 0;
	std::shared_ptr<Transform> transform;
	GLFWwindow* window;

	bool enterKeyWasPressed = false;
};
