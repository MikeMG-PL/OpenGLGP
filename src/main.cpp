#define WINDOW_X 1600
#define WINDOW_Y 900

#include <memory>

#include "GLFW/glfw3.h"
#include "Helpers/GlobalInput.h"
#include "Engine/Editor.h"
#include "Engine/GameInstance.h"
#include "Engine/GameObject.h"
#include "Engine/Renderer.h"
#include "Components/Camera.h"
#include "Components/CameraMovement.h"
#include "Components/DirectionalLight.h"
#include "Components/HutSpawner.h"

int main(int, char**)
{
	if (!Renderer::Get().Init(WINDOW_X, WINDOW_Y))
		return 1;

	GLFWwindow* window = Renderer::Get().GetWindow();
	GameInstance::Get().StartGame();

	/// Declare your GameObjects here ///

	auto camera = GameObject::CreateObject();
	camera->AddComponent<Camera>();
	camera->AddComponent<CameraMovement>();
	camera->GetTransform()->localPosition = { 0, 3, 10 };

	/////////////////////////////////////////////////////////////////////////

	auto dirlight = GameObject::CreateObject();
	dirlight->AddComponent<DirectionalLight>();
	dirlight->GetTransform()->localEulerAngles = {-70, 0, 0};
	dirlight->GetTransform()->localPosition = {25, 25, 25};

	auto ground = GameObject::CreateObject();
	ground->AddComponent<Model>("../../res/models/ground/hut.dae", glm::vec2(128, 128));
	ground->GetTransform()->localScale = { 70, 0.1f, 70 };
	ground->GetTransform()->localPosition = { 500, -2.01f, 500 };

	/// Declare last - instanced ///

	auto hutSpawner = GameObject::CreateObject();
	hutSpawner->AddComponent<HutSpawner>(20);

	// Main loop
	while (!Renderer::Get().ShouldCloseWindow())
	{
		GameInstance::Get().Count();
		globalInput(window);

		Editor::Get().ClearBuffer();
		glfwPollEvents();
		Editor::Get().Update();

		// Rendering code inside Render function
		// You will need to iterate through each Mesh in this function just as GameInstance's UpdateGame does to render it
		Renderer::Get().Render(*camera->GetComponent<Camera>());

		// Update GameObjects
		GameInstance::Get().UpdateGame();
		GameInstance::Get().UpdateGameFixed();

		// Unbind everything here
		// Renderer::Get().Unbind();

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
