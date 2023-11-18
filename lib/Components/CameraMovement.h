#pragma once
#include "Camera.h"
#include "Transform.h"
#include "Engine/Component.h"

struct GLFWwindow;

class CameraMovement : public Component
{
public:

	void Start() override;
	void Update() override;
	float cameraSpeed = 2.0f;

private:

	GLFWwindow* window;
	std::shared_ptr<Transform> cameraTransform;
	std::shared_ptr<Camera> camera;

	void processInput();

};
