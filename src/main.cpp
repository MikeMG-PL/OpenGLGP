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

	// TODO: Make Editor and Renderer parts of GameInstance, preferably a singleton

	// Main loop
	while (!GameInstance::Get().ShouldCloseWindow())
	{
		processInput(GameInstance::Get().GetWindow());

		Editor::Get().ClearBuffer();
		glfwPollEvents();
		Editor::Get().Update();
		
		int display_w, display_h;
		glfwMakeContextCurrent(GameInstance::Get().GetWindow());
		glfwGetFramebufferSize(GameInstance::Get().GetWindow(), &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		Renderer::Get().BackgroundColor(Editor::Get().GetBackgroundColor());

		// Rendering code inside Render function
		Renderer::Get().Render();
		Renderer::Get().Unbind();

		// Draw ImGui
		Editor::Get().RenderDrawData();

		// Swap context and buffer
		glfwMakeContextCurrent(GameInstance::Get().GetWindow());
		glfwSwapBuffers(GameInstance::Get().GetWindow());
	}

	// Cleanup
	Editor::Get().Cleanup();
	glfwDestroyWindow(GameInstance::Get().GetWindow());
	glfwTerminate();

	return 0;
}