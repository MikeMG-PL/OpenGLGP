#include "Engine/Editor.h"

#include <iostream>
#include <glad/glad.h>

#include "imgui.h"
#include "Components/Hut.h"
#include "Components/HutSpawner.h"
#include "Engine/GameInstance.h"
#include "Engine/GameObject.h"
#include "Engine/Renderer.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include "glm/gtx/matrix_decompose.hpp"

Editor& Editor::Get()
{
	static Editor instance;
	return instance;
}

void Editor::Init()
{
	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(Renderer::Get().GetWindow(), true);
	ImGui_ImplOpenGL3_Init(Renderer::Get().GetGLSLVersion());

	// Setup style
	ImGui::StyleColorsDark();
}

void Editor::Update()
{
	// Poll and handle events (inputs, window resize, etc.)
	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.k
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);

	std::shared_ptr<HutSpawner> hcopy;

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	if (showToolWindow)
	{
		for (auto& c : GameInstance::Get().allComponents)
		{
			if ((hut = std::dynamic_pointer_cast<HutSpawner>(c)))
			{
				hutsNum = hut->numInstances;
				hutMatrices = hut->matrices;
				wallMatrices = hut->wallMatrices;
				rootMatrices = hut->rootMatrices;
				roofMatrices = hut->roofMatrices;
				initialRoofAngle = hut->initialRoofAngle;
				initialRoofTranslation = hut->initialRoofTranslation;
				initialRoofAngleAxis = hut->initialRoofAxisAngle;
				hcopy = std::dynamic_pointer_cast<HutSpawner>(c);
			}
		}

		hut = hcopy;

		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Tool window");	// Create a window
		ImGui::ColorEdit3("Background color", reinterpret_cast<float*>(&clearColor)); // Edit 3 floats representing a color
		ImGui::SliderFloat("Camera sensitivity", &cameraSensitivity, 1, 10);
		ImGui::SliderFloat("Movement speed", &cameraSpeed, 5, 25);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Frametime: %.3f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Press [SPACE] to toggle cursor.");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Text("Hut instances: %i", hutsNum);
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::CollapsingHeader("SCENE GRAPH FOR HUTS"))
		{
			ImGui::Indent(20);
			hutTransform();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		////////////
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
}

void Editor::ClearBuffer()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Editor::RenderDrawData()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

ImVec4 Editor::GetBackgroundColor() const
{
	return clearColor;
}

ImVec4 Editor::GetDrawingColor() const
{
	return drawingColor;
}

void Editor::hutTransform()
{
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::InputInt("Hut ID", &editID);
	editID = glm::clamp(editID, 0, hutsNum);
	std::string s = "Hut" + std::to_string(editID);
	if (ImGui::CollapsingHeader(s.c_str()))
	{
		auto transform = hut->rootGameObjects[editID]->GetTransform();

		glm::vec3& hutPos = transform->localPosition;
		float position[3] = { hutPos.x, hutPos.y, hutPos.z };
		ImGui::InputFloat3((s + " position").c_str(), position);
		hutPos.x = position[0];
		hutPos.y = position[1];
		hutPos.z = position[2];

		// Convert quaternion to Euler angles
		glm::vec3& euler = transform->localEulerAngles;
		float rotation[3] = { euler.x, euler.y, euler.z };
		ImGui::InputFloat3((s + " rotation").c_str(), rotation);
		euler.x = rotation[0];
		euler.y = rotation[1];
		euler.z = rotation[2];

		// Input for scale
		glm::vec3& scale = transform->localScale;
		float scaling[3] = { scale.x, scale.y, scale.z };
		ImGui::InputFloat3((s + " scale").c_str(), scaling);
		scale.x = scaling[0];
		scale.y = scaling[1];
		scale.z = scaling[2];

		hut->UpdateTransforms(editID);

		ImGui::Indent(20);

		if (ImGui::CollapsingHeader((s + " walls").c_str()))
		{
			auto transform = hut->rootGameObjects[editID]->GetTransform()->GetChildren()[0];

			glm::vec3& hutPos = transform->localPosition;
			float position[3] = { hutPos.x, hutPos.y, hutPos.z };
			ImGui::InputFloat3((s + "walls position").c_str(), position);
			hutPos.x = position[0];
			hutPos.y = position[1];
			hutPos.z = position[2];

			// Convert quaternion to Euler angles
			glm::vec3& euler = transform->localEulerAngles;
			float rotation[3] = { euler.x, euler.y, euler.z };
			ImGui::InputFloat3((s + "walls rotation").c_str(), rotation);
			euler.x = rotation[0];
			euler.y = rotation[1];
			euler.z = rotation[2];

			// Input for scale
			glm::vec3& scale = transform->localScale;
			float scaling[3] = { scale.x, scale.y, scale.z };
			ImGui::InputFloat3((s + "walls scale").c_str(), scaling);
			scale.x = scaling[0];
			scale.y = scaling[1];
			scale.z = scaling[2];

			hut->UpdateTransforms(editID);
		}

		if (ImGui::CollapsingHeader((s + " roofs").c_str()))
		{
			auto transform = hut->rootGameObjects[editID]->GetTransform()->GetChildren()[1];

			glm::vec3& hutPos = transform->localPosition;
			float position[3] = { hutPos.x, hutPos.y, hutPos.z };
			ImGui::InputFloat3((s + "roofs position").c_str(), position);
			hutPos.x = position[0];
			hutPos.y = position[1];
			hutPos.z = position[2];

			// Convert quaternion to Euler angles
			glm::vec3& euler = transform->localEulerAngles;
			float rotation[3] = { euler.x, euler.y, euler.z };
			ImGui::InputFloat3((s + "roofs rotation").c_str(), rotation);
			euler.x = rotation[0];
			euler.y = rotation[1];
			euler.z = rotation[2];

			// Input for scale
			glm::vec3& scale = transform->localScale;
			float scaling[3] = { scale.x, scale.y, scale.z };
			ImGui::InputFloat3((s + "roofs scale").c_str(), scaling);
			scale.x = scaling[0];
			scale.y = scaling[1];
			scale.z = scaling[2];

			hut->UpdateTransforms(editID);
		}


		ImGui::Indent(-20);
		ImGui::Spacing();
		ImGui::Spacing();
	}
	// }
}
