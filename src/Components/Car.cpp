#include "Components/Car.h"

#include "Engine/GameObject.h"
#include "Engine/Renderer.h"
#include "GLFW/glfw3.h"
#include "Helpers/MathHelpers.h"

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

	if (Renderer::Get().GetCamera()->cameraMode == CAR)
		steering();
}

void Car::processInput()
{
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

void Car::steering()
{
	// Forward, back
	float acc = GameInstance::Get().GetDeltaTime() * 4.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		currentSpeed = glm::clamp(currentSpeed + acc, -maxSpeed, maxSpeed);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		currentSpeed = glm::clamp(currentSpeed - acc, -maxSpeed, maxSpeed);
	}
	else // Neither 'W' nor 'S' is pressed
	{
		if (currentSpeed > 0.0f)
		{
			currentSpeed = glm::clamp(currentSpeed - acc, 0.0f, currentSpeed);
		}
		else if (currentSpeed < 0.0f)
		{
			currentSpeed = glm::clamp(currentSpeed + acc, currentSpeed, 0.0f);
		}
	}

	const glm::vec3 v = glm::normalize(glm::vec3(1, 0, 1));
	const glm::vec3 dirVec = {glm::sin(glm::radians(- transform->localEulerAngles.y)), 0, glm::sin(glm::radians(transform->localEulerAngles.y + 90))};
	const glm::vec3 front = currentSpeed * acc * dirVec;
	const float turnValue = glm::clamp(10 * GameInstance::Get().GetDeltaTime() * currentSpeed, -1.0f, 1.0f);

	// Left, right
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		transform->localEulerAngles.y -= turnValue;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		transform->localEulerAngles.y += turnValue;
	}

	transform->localPosition += front;
}


