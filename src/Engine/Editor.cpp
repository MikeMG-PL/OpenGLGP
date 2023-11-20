#include "Engine/Editor.h"

#include <glad/glad.h>

#include "imgui.h"
#include "Engine/GameInstance.h"
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
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Tool window");                          // Create a window
		ImGui::ColorEdit3("Background color", (float*)&clearColor); // Edit 3 floats representing a color
		ImGui::SliderInt("Generated details", &details, 2, 16);
		ImGui::SliderFloat("Scene rotation", &sunRotation, -180, 180);
		ImGui::Checkbox("Wireframe", &wireframe);

		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		ImGui::Text("Frametime: %.3f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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
