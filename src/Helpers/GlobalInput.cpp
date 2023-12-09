#include "Helpers/GlobalInput.h"

#include "Components/CameraMovement.h"
#include "Engine/GameInstance.h"
#include "Engine/Renderer.h"
#include "GLFW/glfw3.h"

void globalInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Renderer::Get().CloseWindow();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		std::shared_ptr<CameraMovement> cam;
		for (auto& g : GameInstance::Get().allComponents)
		{
			if ((cam = std::dynamic_pointer_cast<CameraMovement>(g)))
				cam->SwitchInput();
		}
	}
}