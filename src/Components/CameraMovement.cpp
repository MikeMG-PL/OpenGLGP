#include "Components/CameraMovement.h"

#include "Engine/Editor.h"
#include "Engine/GameInstance.h"
#include "Engine/GameObject.h"
#include "GLFW/glfw3.h"

#include "Engine/Renderer.h"

void CameraMovement::Start()
{
	Component::Start();
	window = Renderer::Get().GetWindow();
	cameraTransform = GetParent()->GetTransform();
	camera = GetParent()->GetComponent<Camera>();
	glfwGetCursorPos(window, &mouseX, &mouseY);
	lastMouseX = mouseX;
	lastMouseY = mouseY;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void CameraMovement::Update()
{
	Component::Update();
	baseCameraSpeed = Editor::Get().cameraSpeed;
	mouseSensitivity = Editor::Get().cameraSensitivity;

	if (camera->cameraMode == FLYING)
		processInput();
	else
		processDrivingInput();
}

void CameraMovement::SwitchInput()
{
	if (cursorHidden)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		cursorHidden = false;
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, lastMouseX, lastMouseY);
		cursorHidden = true;
	}
}

void CameraMovement::processInput()
{
	if (!cursorHidden)
		return;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed = baseCameraSpeed * 4.0f;
	else
		cameraSpeed = baseCameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraTransform->localPosition += cameraSpeed * camera->GetFront() * GameInstance::Get().GetDeltaTime();
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraTransform->localPosition -= cameraSpeed * camera->GetFront() * GameInstance::Get().GetDeltaTime();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraTransform->localPosition -= glm::normalize(glm::cross(camera->GetFront(), camera->GetWorldUp())) * cameraSpeed * GameInstance::Get().GetDeltaTime();
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraTransform->localPosition += glm::normalize(glm::cross(camera->GetFront(), camera->GetWorldUp())) * cameraSpeed * GameInstance::Get().GetDeltaTime();
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraTransform->localPosition -= cameraSpeed * camera->GetWorldUp() * GameInstance::Get().GetDeltaTime();
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraTransform->localPosition += cameraSpeed * camera->GetWorldUp() * GameInstance::Get().GetDeltaTime();

	// Get the current cursor position
	double currentMouseX, currentMouseY;
	glfwGetCursorPos(window, &currentMouseX, &currentMouseY);

	// Calculate mouse movement since last frame
	double deltaX = currentMouseX - lastMouseX;
	double deltaY = currentMouseY - lastMouseY;

	// Update last mouse position
	lastMouseX = currentMouseX;
	lastMouseY = currentMouseY;

	// Apply mouse sensitivity
	deltaX *= mouseSensitivity;
	deltaY *= mouseSensitivity;

	// Update camera rotation
	cameraTransform->localEulerAngles.y += static_cast<float>(deltaX) * GameInstance::Get().GetDeltaTime();
	cameraTransform->localEulerAngles.x -= static_cast<float>(deltaY) * GameInstance::Get().GetDeltaTime();

	// Clamp camera pitch to prevent flip over
	cameraTransform->localEulerAngles.x = glm::clamp(cameraTransform->localEulerAngles.x, -89.0f, 89.0f);
}

void CameraMovement::processDrivingInput()
{
	cameraTransform->localEulerAngles = { 0,0,0 };

	if (!cursorHidden)
		return;

	// Get the current cursor position
	double currentMouseX, currentMouseY;
	glfwGetCursorPos(window, &currentMouseX, &currentMouseY);

	// Calculate mouse movement since last frame
	double deltaX = currentMouseX - lastMouseX;
	double deltaY = currentMouseY - lastMouseY;

	// Update last mouse position
	lastMouseX = currentMouseX;
	lastMouseY = currentMouseY;

	// Apply mouse sensitivity
	deltaX *= mouseSensitivity;
	deltaY *= mouseSensitivity;

	// // Update camera rotation
	// cameraTransform->localEulerAngles.y += static_cast<float>(deltaX) * GameInstance::Get().GetDeltaTime();
	// cameraTransform->localEulerAngles.x -= static_cast<float>(deltaY) * GameInstance::Get().GetDeltaTime();
	//
	// // Clamp camera pitch to prevent flip over
	// cameraTransform->localEulerAngles.x = glm::clamp(cameraTransform->localEulerAngles.x, -89.0f, 89.0f);
}
