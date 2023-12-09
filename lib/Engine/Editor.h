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
	float cameraSpeed = 4.0;
	float cameraSensitivity = 5.0f;

private:

	bool showDemoWindow = false;
	bool showToolWindow = true;
	ImVec4 clearColor = ImVec4(0, 0, 0.06f, 1.00f);
	ImVec4 drawingColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	
	Editor() = default;

};

