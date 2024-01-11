#define WINDOW_X 1600
#define WINDOW_Y 900

#include <memory>

#include "Animation system/RiggedModel.h"
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
#include "Components/Planet.h"
#include "imgui_impl/imgui_impl_glfw.h"

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

	auto enemy = GameObject::CreateObject();
	enemy->AddComponent<RiggedModel>("../../res/models/enemy/enemy.gltf", "../../res/models/enemy/enemy_pose.gltf");
	// enemy->AddComponent<RiggedModel>("../../res/models/enemy/enemy.dae", "../../res/models/enemy/enemy_pose.dae");
	enemy->GetTransform()->localPosition = glm::vec3(-5, -1, 0);

	// auto enemyDummy = GameObject::CreateObject();
	// enemyDummy->AddComponent<Model>("../../res/models/enemy/enemy.gltf", glm::vec2(1, 1));
	// enemyDummy->GetTransform()->localPosition = glm::vec3(-8, -1, 0);

	auto dirlight = GameObject::CreateObject();
	dirlight->AddComponent<DirectionalLight>();
	dirlight->GetTransform()->localEulerAngles = {-70, 0, 0};
	dirlight->GetTransform()->localPosition = {25, 25, 25};

	auto pointLightRoot = GameObject::CreateObject();
	pointLightRoot->GetTransform()->localPosition = { 50, 3, 50 };
	pointLightRoot->AddComponent<Planet>(10);

	auto pointLight = GameObject::CreateObject();
	pointLight->AddComponent<PointLight>();
	pointLight->GetTransform()->localPosition = { 30, 0, 0 };

	pointLightRoot->GetTransform()->AddChild(pointLight->GetTransform());

	auto spotLight1 = GameObject::CreateObject();
	spotLight1->AddComponent<SpotLight>();
	spotLight1->GetTransform()->localPosition = { 20, 15, 60 };
	spotLight1->GetTransform()->localEulerAngles = { -30, 0, 0 };

	auto spotLight2 = GameObject::CreateObject();
	spotLight2->AddComponent<SpotLight>();
	spotLight2->GetTransform()->localPosition = { 50, 15, 30 };
	spotLight2->GetTransform()->localEulerAngles = { -150, 0, 0 };

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
