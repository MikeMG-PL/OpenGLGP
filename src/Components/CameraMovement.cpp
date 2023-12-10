#include "Components/CameraMovement.h"

#include "Engine/Editor.h"
#include "Engine/GameInstance.h"
#include "Engine/GameObject.h"
#include "GLFW/glfw3.h"

#include "Engine/Renderer.h"
#include "Helpers/GlobalInput.h"

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
	glfwSetKeyCallback(window, &keyCallback);
}

void CameraMovement::Update()
{
	Component::Update();
	baseCameraSpeed = Editor::Get().cameraSpeed;
	mouseSensitivity = Editor::Get().cameraSensitivity;
	processInput();
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
		cameraTransform->setLocalPosition(cameraTransform->getLocalPosition() + cameraSpeed * camera->GetFront() * GameInstance::Get().GetDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraTransform->setLocalPosition(cameraTransform->getLocalPosition() - cameraSpeed * camera->GetFront() * GameInstance::Get().GetDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraTransform->setLocalPosition(cameraTransform->getLocalPosition() - glm::normalize(glm::cross(camera->GetFront(), camera->GetWorldUp())) * cameraSpeed * GameInstance::Get().GetDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraTransform->setLocalPosition(cameraTransform->getLocalPosition() + glm::normalize(glm::cross(camera->GetFront(), camera->GetWorldUp())) * cameraSpeed * GameInstance::Get().GetDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraTransform->setLocalPosition(cameraTransform->getLocalPosition() - cameraSpeed * camera->GetWorldUp() * GameInstance::Get().GetDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraTransform->setLocalPosition(cameraTransform->getLocalPosition() + cameraSpeed * camera->GetWorldUp() * GameInstance::Get().GetDeltaTime());


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
	cameraTransform->setLocalEulerAngles(glm::vec3(
		cameraTransform->getLocalEulerAngles().x - static_cast<float>(deltaY) * GameInstance::Get().GetDeltaTime(),
		cameraTransform->getLocalEulerAngles().y + static_cast<float>(deltaX) * GameInstance::Get().GetDeltaTime(),
		cameraTransform->getLocalEulerAngles().z
	));

	// Clamp camera pitch to prevent flip over
	cameraTransform->setLocalEulerAngles(glm::vec3(
		glm::clamp(cameraTransform->getLocalEulerAngles().x, -89.0f, 89.0f),
		cameraTransform->getLocalEulerAngles().y,
		cameraTransform->getLocalEulerAngles().z
	));

}
