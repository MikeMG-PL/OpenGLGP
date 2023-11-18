#include "Components/CameraMovement.h"

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
}

void CameraMovement::Update()
{
	Component::Update();
	processInput();
}

void CameraMovement::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraTransform->position += cameraSpeed * camera->GetWorldFront() * GameInstance::Get().GetDeltaTime();
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraTransform->position -= cameraSpeed * camera->GetWorldFront() * GameInstance::Get().GetDeltaTime();
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraTransform->position -= glm::normalize(glm::cross(camera->GetWorldFront(), camera->GetWorldUp())) * cameraSpeed * GameInstance::Get().GetDeltaTime();
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraTransform->position += glm::normalize(glm::cross(camera->GetWorldFront(), camera->GetWorldUp())) * cameraSpeed * GameInstance::Get().GetDeltaTime();
}
