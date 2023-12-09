#pragma once
#include <imgui.h>
#include <memory>
#include <vector>

#include "Components/Transform.h"

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
	float cameraSpeed = 8.0;
	float cameraSensitivity = 6.0f;

private:

	std::vector<std::shared_ptr<GameObject>> huts;
	bool showDemoWindow = false;
	bool showToolWindow = true;
	ImVec4 clearColor = ImVec4(0, 0, 0.06f, 1.00f);
	ImVec4 drawingColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	void hutTransform(std::vector<std::shared_ptr<GameObject>>& hutGameObjects, int hutsNumber);
	
	Editor() = default;

};

