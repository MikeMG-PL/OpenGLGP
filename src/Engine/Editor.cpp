#include "Engine/Editor.h"

#include <iostream>
#include <glad/glad.h>

#include "imgui.h"
#include "Components/Hut.h"
#include "Engine/GameInstance.h"
#include "Engine/GameObject.h"
#include "Engine/Renderer.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

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

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	if (showToolWindow)
	{
		huts.clear();
		std::shared_ptr<Hut> hut;
		int hutNum = 0;
		for (auto& c : GameInstance::Get().allComponents)
		{
			if ((hut = std::dynamic_pointer_cast<Hut>(c)))
			{
				hutNum++;
				huts.push_back(hut->GetParent());
			}
		}

		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Tool window");	// Create a window
		ImGui::ColorEdit3("Background color", reinterpret_cast<float*>(&clearColor)); // Edit 3 floats representing a color
		ImGui::SliderFloat("Camera sensitivity", &cameraSensitivity, 1, 10);
		ImGui::SliderFloat("Movement speed", &cameraSpeed, 1, 10);
		ImGui::Checkbox("Wireframe", &wireframe);

		ImGui::Spacing();
		ImGui::Spacing();

		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Frametime: %.3f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Press [SPACE] to toggle cursor.");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Text("Hut instances: %i", hutNum);
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::CollapsingHeader("SCENE GRAPH FOR HUTS"))
		{
			ImGui::Indent(20);
			hutTransform(huts, hutNum);
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

void Editor::hutTransform(std::vector<std::shared_ptr<GameObject>>& hutGameObjects, int hutsNumber)
{
	for (int i = 0; i < hutsNumber; i++)
	{
		std::string s = "Hut" + std::to_string(i);
		if (ImGui::CollapsingHeader(s.c_str()))
		{
			glm::vec3& hutPos = hutGameObjects[i]->GetTransform()->localPosition;
			float position[3] = { hutPos.x, hutPos.y, hutPos.z };
			ImGui::InputFloat3((s + " position").c_str(), position);
			hutPos.x = position[0];
			hutPos.y = position[1];
			hutPos.z = position[2];
			hutGameObjects[i]->GetTransform()->localPosition = hutPos;

			glm::vec3& hutAngles = hutGameObjects[i]->GetTransform()->localEulerAngles;
			float angles[3] = { hutAngles.x, hutAngles.y, hutAngles.z };
			ImGui::InputFloat3((s + " rotation").c_str(), angles);
			hutAngles.x = angles[0];
			hutAngles.y = angles[1];
			hutAngles.z = angles[2];
			hutGameObjects[i]->GetTransform()->localEulerAngles = hutAngles;

			glm::vec3& hutScale = hutGameObjects[i]->GetTransform()->localScale;
			float scale[3] = { hutScale.x, hutScale.y, hutScale.z };
			ImGui::InputFloat3((s + " scale").c_str(), scale);
			hutScale.x = scale[0];
			hutScale.y = scale[1];
			hutScale.z = scale[2];
			hutGameObjects[i]->GetTransform()->localScale = hutScale;

			ImGui::Indent(20);

			if (ImGui::CollapsingHeader((s + " walls").c_str()))
			{
				const std::shared_ptr<GameObject> hut = hutGameObjects[i];
				if(hut != nullptr)
				{
					glm::vec3& wallsPos = hut->GetComponent<Hut>()->hutPtr->GetTransform()->localPosition;
					float position1[3] = { wallsPos.x, wallsPos.y, wallsPos.z };
					ImGui::InputFloat3((s + " wall position").c_str(), position1);
					wallsPos.x = position1[0];
					wallsPos.y = position1[1];
					wallsPos.z = position1[2];
				}
			}
			if (ImGui::CollapsingHeader((s + " roof").c_str()))
			{
				const std::shared_ptr<GameObject> hut = hutGameObjects[i];
				if (hut != nullptr)
				{
					glm::vec3& roofPos = hut->GetComponent<Hut>()->roofPtr->GetTransform()->localPosition;
					float position2[3] = { roofPos.x, roofPos.y, roofPos.z };
					ImGui::InputFloat3((s + " roof position").c_str(), position2);
					roofPos.x = position2[0];
					roofPos.y = position2[1];
					roofPos.z = position2[2];
				}
			}

			ImGui::Indent(-20);
			ImGui::Spacing();
			ImGui::Spacing();
		}
	}
}
