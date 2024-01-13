#include "Components/Car.h"

#include "Engine/GameObject.h"
#include "Engine/Renderer.h"
#include "GLFW/glfw3.h"

void Car::Start()
{
	Component::Start();
	transform = GetParent()->GetTransform();
	window = Renderer::Get().GetWindow();
}

void Car::Update()
{
	Component::Update();
	processInput();
	transform->localPosition.y += 0.3f * GameInstance::Get().GetDeltaTime();
}

void Car::processInput()
{
	// if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	// 	cameraTransform->localPosition += cameraSpeed * camera->GetFront() * GameInstance::Get().GetDeltaTime();
	// if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	// 	cameraTransform->localPosition -= cameraSpeed * camera->GetFront() * GameInstance::Get().GetDeltaTime();
	// if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	// 	cameraTransform->localPosition -= glm::normalize(glm::cross(camera->GetFront(), camera->GetWorldUp())) * cameraSpeed * GameInstance::Get().GetDeltaTime();
	// if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	// 	cameraTransform->localPosition += glm::normalize(glm::cross(camera->GetFront(), camera->GetWorldUp())) * cameraSpeed * GameInstance::Get().GetDeltaTime();

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !enterKeyWasPressed)
	{
		if (Renderer::Get().GetCamera()->cameraMode == FLYING)
		{
			Renderer::Get().GetCamera()->cameraMode = CAR;
			transform->AddChild(Renderer::Get().GetCamera()->GetParent()->GetTransform());
			Renderer::Get().GetCamera()->GetParent()->GetTransform()->localPosition = cameraOffset;
		}
		else if (Renderer::Get().GetCamera()->cameraMode == CAR)
		{
			Renderer::Get().GetCamera()->cameraMode = FLYING;
			transform->RemoveLastChild();
			Renderer::Get().GetCamera()->GetParent()->GetTransform()->localPosition = {0,8,0};
		}
		enterKeyWasPressed = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
	{
		enterKeyWasPressed = false;
	}
}
