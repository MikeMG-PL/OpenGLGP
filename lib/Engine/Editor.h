#pragma once
#include <imgui.h>
#include <memory>
#include <vector>

#include "Components/DirectionalLight.h"
#include "Components/HutSpawner.h"
#include "Components/PointLight.h"
#include "Components/SpotLight.h"
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
	float cameraSpeed = 6.0;
	float cameraSensitivity = 6.0f;
	int editID = 0;

private:

	bool showDemoWindow = false;
	bool showToolWindow = true;

	ImVec4 clearColor = ImVec4(0.17f, 0.17f, 0.34f, 1.00f);
	ImVec4 drawingColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

	// Directional Light
	DirectionalLightParams* d;

	std::vector<PointLightParams*> ps;
	std::vector<SpotLightParams*> ss;

	glm::mat4* hutMatrices;
	glm::mat4* wallMatrices;
	glm::mat4* roofMatrices;
	glm::mat4* rootMatrices;
	glm::vec3 initialRoofTranslation;
	glm::vec3 initialRoofAngleAxis;
	float initialRoofAngle;
	std::shared_ptr<HutSpawner> hut;
	std::shared_ptr<DirectionalLight> dirLight;
	std::vector<std::shared_ptr<PointLight>> pointLights;
	std::vector<std::shared_ptr<SpotLight>> spotLights;
	int hutsNum;
	void hutTransform();
	void dirLightParams();
	void spotLightParams();
	void pointLightParams();
	bool lightsRegistered;
	
	Editor() = default;

};

