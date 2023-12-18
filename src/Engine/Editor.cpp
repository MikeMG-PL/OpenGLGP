#include "Engine/Editor.h"

#include <iostream>
#include <glad/glad.h>

#include "imgui.h"
#include "Components/Hut.h"
#include "Components/HutSpawner.h"
#include "Engine/GameInstance.h"
#include "Engine/GameObject.h"
#include "Engine/Renderer.h"
#include "GLFW/glfw3.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include "glm/gtx/matrix_decompose.hpp"
#include "Helpers/GlobalInput.h"

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

	setKeyCallbacks();

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

			if (!lightsRegistered)
			{
				if (std::dynamic_pointer_cast<DirectionalLight>(c))
				{
					dirLight = std::dynamic_pointer_cast<DirectionalLight>(c);
					d = &dirLight->d;
				}

				if (std::dynamic_pointer_cast<PointLight>(c))
				{
					std::shared_ptr<PointLight> pointLight;
					pointLight = std::dynamic_pointer_cast<PointLight>(c);
					pointLights.emplace_back(pointLight);
					ps.emplace_back(&pointLight->p);
				}

				if (std::dynamic_pointer_cast<SpotLight>(c))
				{
					std::shared_ptr<SpotLight> spotLight;
					spotLight = std::dynamic_pointer_cast<SpotLight>(c);
					spotLights.emplace_back(spotLight);
					ss.emplace_back(&spotLight->s);
				}
			}
		}
		lightsRegistered = true;
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

		dirLightParams();
		spotLightParams();
		pointLightParams();

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
}

void Editor::dirLightParams()
{
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Indent(20);
	ImGui::Text("Directional light");

	const auto transform = dirLight->GetParent()->GetTransform();

	// Convert quaternion to Euler angles
	glm::vec3& euler = transform->localEulerAngles;
	float rotation[3] = { euler.x, euler.y, euler.z };
	ImGui::InputFloat3("Directional light rotation", rotation);
	euler.x = rotation[0];
	euler.y = rotation[1];
	euler.z = rotation[2];

	ImGui::ColorEdit3("Directional light ambient", reinterpret_cast<float*>(&d->ambient));
	ImGui::ColorEdit3("Directional light diffuse", reinterpret_cast<float*>(&d->diffuse));
	ImGui::ColorEdit3("Directional light specular", reinterpret_cast<float*>(&d->specular));
	ImGui::Checkbox("Directional light enabled", &d->enabled);

	ImGui::Indent(-20);
	ImGui::Spacing();
	ImGui::Spacing();
}

void Editor::pointLightParams()
{
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Indent(20);

	for (int i = 0; i < pointLights.size(); i++)
	{
		ImGui::Text((std::to_string(i) + " point light").c_str());
		const auto transform = pointLights[i]->GetParent()->GetTransform();

		glm::vec3& pos = transform->localPosition;
		float position[3] = { pos.x, pos.y, pos.z };
		ImGui::InputFloat3((std::to_string(i) + " point light position").c_str(), position);
		pos.x = position[0];
		pos.y = position[1];
		pos.z = position[2];

		// Convert quaternion to Euler angles
		glm::vec3& euler = transform->localEulerAngles;
		float rotation[3] = { euler.x, euler.y, euler.z };
		ImGui::InputFloat3((std::to_string(i) + " point light rotation").c_str(), rotation);
		euler.x = rotation[0];
		euler.y = rotation[1];
		euler.z = rotation[2];

		ImGui::ColorEdit3((std::to_string(i) + " point light ambient").c_str(), reinterpret_cast<float*>(&ps[i]->ambient));
		ImGui::ColorEdit3((std::to_string(i) + " point light diffuse").c_str(), reinterpret_cast<float*>(&ps[i]->diffuse));
		ImGui::ColorEdit3((std::to_string(i) + " point light specular").c_str(), reinterpret_cast<float*>(&ps[i]->specular));
		ImGui::InputFloat((std::to_string(i) + " point light constant").c_str(), &ps[i]->constant);
		ImGui::InputFloat((std::to_string(i) + " point light linear").c_str(), &ps[i]->linear);
		ImGui::InputFloat((std::to_string(i) + " point light quadratic").c_str(), &ps[i]->quadratic);
		ImGui::Checkbox((std::to_string(i) + " point light enabled").c_str(), &ps[i]->enabled);
	}

	ImGui::Indent(-20);
	ImGui::Spacing();
	ImGui::Spacing();
}

void Editor::spotLightParams()
{
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Indent(20);

	for (int i = 0; i < spotLights.size(); i++)
	{
		ImGui::Text((std::to_string(i) + " spot light").c_str());
		const auto transform = spotLights[i]->GetParent()->GetTransform();

		glm::vec3& pos = transform->localPosition;
		float position[3] = { pos.x, pos.y, pos.z };
		ImGui::InputFloat3((std::to_string(i) + " spot light position").c_str(), position);
		pos.x = position[0];
		pos.y = position[1];
		pos.z = position[2];

		// Convert quaternion to Euler angles
		glm::vec3& euler = transform->localEulerAngles;
		float rotation[3] = { euler.x, euler.y, euler.z };
		ImGui::InputFloat3((std::to_string(i) + " spot light rotation").c_str(), rotation);
		euler.x = rotation[0];
		euler.y = rotation[1];
		euler.z = rotation[2];

		ImGui::ColorEdit3((std::to_string(i) + " spotlight ambient").c_str(), reinterpret_cast<float*>(&ss[i]->ambient));
		ImGui::ColorEdit3((std::to_string(i) + " spotlight diffuse").c_str(), reinterpret_cast<float*>(&ss[i]->diffuse));
		ImGui::ColorEdit3((std::to_string(i) + " spotlight specular").c_str(), reinterpret_cast<float*>(&ss[i]->specular));
		ImGui::InputFloat((std::to_string(i) + " spotlight constant").c_str(), &ss[i]->constant);
		ImGui::InputFloat((std::to_string(i) + " spotlight linear").c_str(), &ss[i]->linear);
		ImGui::InputFloat((std::to_string(i) + " spotlight quadratic").c_str(), &ss[i]->quadratic);
		ImGui::InputFloat((std::to_string(i) + " spotlight cutOff").c_str(), &ss[i]->cutOff);
		ImGui::InputFloat((std::to_string(i) + " spotlight outerCutOff").c_str(), &ss[i]->outerCutOff);
		ImGui::Checkbox((std::to_string(i) + " spotlight enabled").c_str(), &ss[i]->enabled);
	}

	ImGui::Indent(-20);
	ImGui::Spacing();
	ImGui::Spacing();
}
