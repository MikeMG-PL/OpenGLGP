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
	void SwitchInput();
	float baseCameraSpeed = 4.0f;
	float mouseSensitivity = 5.0f;
	bool cursorHidden = true;

private:

	double mouseX, mouseY, lastMouseX, lastMouseY;
	bool delay = false;
	GLFWwindow* window;
	std::shared_ptr<Transform> cameraTransform;
	std::shared_ptr<Camera> camera;
	float cameraSpeed = baseCameraSpeed;

	void processInput();
	void processDrivingInput();
};
