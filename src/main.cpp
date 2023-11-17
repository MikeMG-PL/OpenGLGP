#define WINDOW_X 1600
#define WINDOW_Y 900

#include "GLFW/glfw3.h"
#include "Input.h"
#include "Engine/Editor.h"
#include "Engine/GameInstance.h"
#include "Engine/Renderer.h"

// TODO: Prepare "EngineComponent" class which also has Start and Update methods but doesn't require being attached to GameObject

int main(int, char**)
{
	if (!Renderer::Get().Init(WINDOW_X, WINDOW_Y))
		return 1;

	GLFWwindow* window = Renderer::Get().GetWindow();
	GameInstance::Get().StartGame();

	// Main loop
	while (!Renderer::Get().ShouldCloseWindow())
	{
		processInput(window);

		Editor::Get().ClearBuffer();
		glfwPollEvents();
		Editor::Get().Update();

		// Rendering code inside Render function
		Renderer::Get().Render();

		// Unbind everything here
		Renderer::Get().Unbind();

		// Draw ImGui
		Editor::Get().RenderDrawData();

		// Swap context and buffer
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	Editor::Get().Cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}