#include "Helpers/GlobalInput.h"
#include "Engine/GameInstance.h"
#include "Engine/Renderer.h"
#include "GLFW/glfw3.h"

void globalInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Renderer::Get().CloseWindow();
}