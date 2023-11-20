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
	camera->GetTransform()->localPosition = glm::vec3(0, 3, 10);
	camera->GetTransform()->localEulerAngles = glm::vec3(0, 90, 0);

	/////////////////////////////////////////////////////////////////////////

	auto root = GameObject::CreateObject();

	auto sun = GameObject::CreateObject();
	sun->AddComponent<Model>("../../res/models/sun/sun.obj");
	sun->AddComponent<Planet>(30);

	root->GetTransform()->AddChild(sun->GetTransform());
	root->AddComponent<Planet>(0, true);

	auto earthPivot = GameObject::CreateObject();
	earthPivot->AddComponent<Model>(2.5f); // orbit radius
	earthPivot->AddComponent<Planet>(100);

	auto earth = GameObject::CreateObject();
	earth->AddComponent<Model>("../../res/models/earth/earth.obj");
	earth->AddComponent<Planet>(30);

	earth->GetTransform()->localEulerAngles = glm::vec3(0, 0, 25);
	earth->GetTransform()->localPosition = glm::vec3(2.5f, 0, 0);
	earth->GetTransform()->localScale = glm::vec3(0.2f, 0.2f, 0.2f);

	auto moonPivot = GameObject::CreateObject();
	moonPivot->AddComponent<Model>(2.5f);
	moonPivot->AddComponent<Planet>(120);
	moonPivot->GetTransform()->localEulerAngles = glm::vec3(0, 0, 50);

	auto moon = GameObject::CreateObject();
	moon->AddComponent<Model>("../../res/models/moon/moon.obj");
	moon->AddComponent<Planet>(70);

	moon->GetTransform()->localEulerAngles = glm::vec3(0, 0, 50);
	moon->GetTransform()->localPosition = glm::vec3(2.5f, 0, 0);
	moon->GetTransform()->localScale = glm::vec3(0.2f, 0.2f, 0.2f);

	/////////////////////////////////////////////////////////////////////////

	auto generatedPivot = GameObject::CreateObject();
	generatedPivot->AddComponent<Model>(4); // orbit radius
	generatedPivot->AddComponent<Planet>(180);

	auto generated = GameObject::CreateObject();
	generated->AddComponent<Model>(1, Editor::Get().details, Editor::Get().details, "../../res/textures/generated.png", GL_TRIANGLE_STRIP);
	generated->AddComponent<Planet>(37);

	generated->GetTransform()->localEulerAngles = glm::vec3(0, 0, 25);
	generated->GetTransform()->localPosition = glm::vec3(4, 0, 0);
	generated->GetTransform()->localScale = glm::vec3(0.7f, 0.7f, 0.7f);

	auto generatedMoonPivot = GameObject::CreateObject();
	generatedMoonPivot->AddComponent<Model>(4);
	generatedMoonPivot->AddComponent<Planet>(24);
	generatedMoonPivot->GetTransform()->localEulerAngles = glm::vec3(0, 13, 0);

	auto generatedMoon = GameObject::CreateObject();
	generatedMoon->AddComponent<Model>("../../res/models/jupiter/jupiter.obj");
	generatedMoon->AddComponent<Planet>(70);

	generatedMoon->GetTransform()->localEulerAngles = glm::vec3(0, 13, 0);
	generatedMoon->GetTransform()->localPosition = glm::vec3(4, 0, 0);
	generatedMoon->GetTransform()->localScale = glm::vec3(0.35f, 0.35f, 0.35f);

	auto generatedMoonPivot2 = GameObject::CreateObject();
	generatedMoonPivot2->AddComponent<Model>(6);
	generatedMoonPivot2->AddComponent<Planet>(210);
	generatedMoonPivot2->GetTransform()->localEulerAngles = glm::vec3(0, 13, 0);

	auto generatedMoon2 = GameObject::CreateObject();
	generatedMoon2->AddComponent<Model>(1.5f, Editor::Get().details, Editor::Get().details, "../../res/textures/generated.png", GL_TRIANGLE_STRIP);
	generatedMoon2->AddComponent<Planet>(12);

	generatedMoon2->GetTransform()->localEulerAngles = glm::vec3(0, 61, 0);
	generatedMoon2->GetTransform()->localPosition = glm::vec3(6, 0, 0);
	generatedMoon2->GetTransform()->localScale = glm::vec3(0.35f, 0.35f, 0.35f);
	
	/////////////////////////////////////////////////////////////////////////

	auto mercuryPivot = GameObject::CreateObject();
	mercuryPivot->AddComponent<Model>(10); // orbit radius
	mercuryPivot->AddComponent<Planet>(0);

	auto mercury = GameObject::CreateObject();
	mercury->AddComponent<Model>("../../res/models/mercury/mercury.obj");
	mercury->AddComponent<Planet>(30);

	mercury->GetTransform()->localEulerAngles = glm::vec3(0, 0, 25);
	mercury->GetTransform()->localPosition = glm::vec3(0, 0, 10);
	mercury->GetTransform()->localScale = glm::vec3(0.53f, 0.53f, 0.53f);

	auto mercuryMoonPivot = GameObject::CreateObject();
	mercuryMoonPivot->AddComponent<Model>(7);
	mercuryMoonPivot->AddComponent<Planet>(58);
	mercuryMoonPivot->GetTransform()->localEulerAngles = glm::vec3(0, 28, 0);

	auto mercuryMoon = GameObject::CreateObject();
	mercuryMoon->AddComponent<Model>("../../res/models/moon/moon.obj");
	mercuryMoon->AddComponent<Planet>(92);

	mercuryMoon->GetTransform()->localEulerAngles = glm::vec3(0, 28, 0);
	mercuryMoon->GetTransform()->localPosition = glm::vec3(7, 0, 0);
	mercuryMoon->GetTransform()->localScale = glm::vec3(0.16f, 0.16f, 0.16f);

	auto mercuryMoonPivot2 = GameObject::CreateObject();
	mercuryMoonPivot2->AddComponent<Model>(4);
	mercuryMoonPivot2->AddComponent<Planet>(130);
	mercuryMoonPivot2->GetTransform()->localEulerAngles = glm::vec3(0, 0, 60);

	auto mercuryMoon2 = GameObject::CreateObject();
	mercuryMoon2->AddComponent<Model>("../../res/models/moon/moon.obj");
	mercuryMoon2->AddComponent<Planet>(14);

	mercuryMoon2->GetTransform()->localEulerAngles = glm::vec3(0, 0, 60);
	mercuryMoon2->GetTransform()->localPosition = glm::vec3(4, 0, 0);
	mercuryMoon2->GetTransform()->localScale = glm::vec3(0.26f, 0.26f, 0.26f);

	/////////////////////////////////////////////////////////////////////////

	auto venusPivot = GameObject::CreateObject();
	venusPivot->AddComponent<Model>(15); // orbit radius
	venusPivot->AddComponent<Planet>(28);

	auto venus = GameObject::CreateObject();
	venus->AddComponent<Model>("../../res/models/venus/venus.obj");
	venus->AddComponent<Planet>(30);

	venus->GetTransform()->localEulerAngles = glm::vec3(0, 0, 25);
	venus->GetTransform()->localPosition = glm::vec3(15, 0, 0);
	venus->GetTransform()->localScale = glm::vec3(0.21f, 0.21f, 0.21f);

	auto venusMoonPivot = GameObject::CreateObject();
	venusMoonPivot->AddComponent<Model>(10);
	venusMoonPivot->AddComponent<Planet>(158);
	venusMoonPivot->GetTransform()->localEulerAngles = glm::vec3(11, 0, 0);

	auto venusMoon = GameObject::CreateObject();
	venusMoon->AddComponent<Model>("../../res/models/earth/earth.obj");
	venusMoon->AddComponent<Planet>(81);

	venusMoon->GetTransform()->localEulerAngles = glm::vec3(11, 0, 0);
	venusMoon->GetTransform()->localPosition = glm::vec3(10, 0, 0);
	venusMoon->GetTransform()->localScale = glm::vec3(0.19f, 0.19f, 0.19f);

	auto venusMoonPivot2 = GameObject::CreateObject();
	venusMoonPivot2->AddComponent<Model>(6);
	venusMoonPivot2->AddComponent<Planet>(260);
	venusMoonPivot2->GetTransform()->localEulerAngles = glm::vec3(0, 90, 0);

	auto venusMoon2 = GameObject::CreateObject();
	venusMoon2->AddComponent<Model>("../../res/models/mercury/mercury.obj");
	venusMoon2->AddComponent<Planet>(38);

	venusMoon2->GetTransform()->localEulerAngles = glm::vec3(0, 90, 0);
	venusMoon2->GetTransform()->localPosition = glm::vec3(6, 0, 0);
	venusMoon2->GetTransform()->localScale = glm::vec3(0.65, 0.65, 0.65);

	sun->GetTransform()->AddChild(earthPivot->GetTransform());
	/**/ earthPivot->GetTransform()->AddChild(earth->GetTransform());
	/**/ /**/ earth->GetTransform()->AddChild(moonPivot->GetTransform());
	/**/ /**/ /**/ moonPivot->GetTransform()->AddChild(moon->GetTransform());

	sun->GetTransform()->AddChild(generatedPivot->GetTransform());
	/**/ generatedPivot->GetTransform()->AddChild(generated->GetTransform());
	/**/ /**/ generated->GetTransform()->AddChild(generatedMoonPivot->GetTransform());
	/**/ /**/ /**/ generatedMoonPivot->GetTransform()->AddChild(generatedMoon->GetTransform());
	/**/ /**/ generated->GetTransform()->AddChild(generatedMoonPivot2->GetTransform());
	/**/ /**/ /**/ generatedMoonPivot2->GetTransform()->AddChild(generatedMoon2->GetTransform());

	sun->GetTransform()->AddChild(mercuryPivot->GetTransform());
	/**/ mercuryPivot->GetTransform()->AddChild(mercury->GetTransform());
	/**/ /**/ mercury->GetTransform()->AddChild(mercuryMoonPivot->GetTransform());
	/**/ /**/ /**/ mercuryMoonPivot->GetTransform()->AddChild(mercuryMoon->GetTransform());
	/**/ /**/ mercury->GetTransform()->AddChild(mercuryMoonPivot2->GetTransform());
	/**/ /**/ /**/ mercuryMoonPivot2->GetTransform()->AddChild(mercuryMoon2->GetTransform());

	sun->GetTransform()->AddChild(venusPivot->GetTransform());
	/**/ venusPivot->GetTransform()->AddChild(venus->GetTransform());
	/**/ /**/ venus->GetTransform()->AddChild(venusMoonPivot->GetTransform());
	/**/ /**/ /**/ venusMoonPivot->GetTransform()->AddChild(venusMoon->GetTransform());
	/**/ /**/ venus->GetTransform()->AddChild(venusMoonPivot2->GetTransform());
	/**/ /**/ /**/ venusMoonPivot2->GetTransform()->AddChild(venusMoon2->GetTransform());

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
