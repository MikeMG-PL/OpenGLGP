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
	if (!GameInstance::Get().StartGame(WINDOW_X, WINDOW_Y))
		return 1;

	Renderer::Get().Prepare();
	GLFWwindow* window = GameInstance::Get().GetWindow();

	// TODO: Make Editor and Renderer parts of GameInstance, preferably a singleton

	// Main loop
	while (!GameInstance::Get().ShouldCloseWindow())
	{
		processInput(window);

		Editor::Get().ClearBuffer();
		glfwPollEvents();
		Editor::Get().Update();
		Renderer::Get().SetBackgroundColor(Editor::Get().GetBackgroundColor());

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