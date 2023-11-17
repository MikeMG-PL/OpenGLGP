#include "Input.h"
#include "Engine/GameInstance.h"
#include "Engine/Renderer.h"
#include "GLFW/glfw3.h"

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Renderer::Get().CloseWindow();
}