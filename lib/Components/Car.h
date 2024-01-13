#pragma once
#include "Transform.h"
#include "Engine/Component.h"
#include "GLFW/glfw3.h"

class Car : public Component
{
public:

	void Start() override;
	void Update() override;

	glm::vec3 cameraOffset = { 0, 3, 0 };

private:

	void processInput();
	std::shared_ptr<Transform> transform;
	GLFWwindow* window;
};
