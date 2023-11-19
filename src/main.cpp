#define WINDOW_X 1600
#define WINDOW_Y 900

#include "GLFW/glfw3.h"
#include "Helpers/GlobalInput.h"
#include "Engine/Editor.h"
#include "Engine/GameInstance.h"
#include "Engine/GameObject.h"
#include "Engine/Renderer.h"
#include "Components/Camera.h"
#include "Components/CameraMovement.h"
#include "Components/Model.h"
#include "Components/Planet.h"

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
	camera->GetTransform()->localPosition = glm::vec3(0, 0, 3);

	auto sun = GameObject::CreateObject();
	sun->AddComponent<Model>("../../res/models/sun/sun.obj");
	sun->AddComponent<Planet>(10);

	auto earthPivot = GameObject::CreateObject();
	earthPivot->AddComponent<Planet>(30);
	earthPivot->GetTransform()->localEulerAngles = glm::vec3(0, 0, 65);

	auto earth = GameObject::CreateObject();
	earth->AddComponent<Model>("../../res/models/earth/earth.obj");
	earth->GetTransform()->localEulerAngles = glm::vec3(0, 0, 25);
	earth->GetTransform()->localPosition = glm::vec3(2.5f, 0, 0);
	earth->GetTransform()->localScale = glm::vec3(0.2f, 0.2f, 0.2f);

	earthPivot->GetTransform()->AddChild(earth->GetTransform());

	auto moon = GameObject::CreateObject();
	moon->AddComponent<Model>("../../res/models/jupiter/jupiter.obj");
	moon->AddComponent<Planet>(50);
	moon->GetTransform()->localPosition = glm::vec3(2.5f, 0, 0);
	moon->GetTransform()->localScale = glm::vec3(0.2f, 0.2f, 0.2f);

	earth->GetTransform()->AddChild(moon->GetTransform());

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
