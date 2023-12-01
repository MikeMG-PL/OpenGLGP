#pragma once
#include <imgui.h>

class Editor
{
public:

	Editor& operator=(const Editor&) = delete;
	Editor(const Editor&) = delete;
	//~Editor();
	static Editor& Get();

	void Init();
	void Update();
	void ClearBuffer();
	void RenderDrawData();
	void Cleanup();
	ImVec4 GetBackgroundColor() const;
	ImVec4 GetDrawingColor() const;
	bool wireframe;
	int details = 16;
	int previousDetails = -1;
	float sunRotation = 0;

private:

	bool showDemoWindow = false;
	bool showToolWindow = true;
	ImVec4 clearColor = ImVec4(0.5f, 0.5f, 0.5f, 1);
	ImVec4 drawingColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	
	Editor() = default;

};

