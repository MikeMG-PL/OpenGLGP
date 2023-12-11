#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Components/Hut.h"
#include "Model.h"
#include "Engine/Component.h"
#include "Engine/GameObject.h"

class HutSpawner : public Component
{
public:

	HutSpawner(int instancesInRow);
	~HutSpawner() override;
	void Start() override;
	void Draw(Shader shader, int instanceID);
	void UpdateTransforms(int n);

	glm::vec3 initialRoofTranslation = {0, 10, 0};
	glm::vec3 initialRoofAxisAngle = {1, 0, 0};
	float initialRoofAngle = -90.0f;
	glm::vec3 initialRoofScale = {};

	int hutOffset = 3;
	glm::mat4* matrices;
	glm::mat4* roofMatrices;
	glm::mat4* wallMatrices;
	glm::mat4* rootMatrices;
	std::vector<std::shared_ptr<GameObject>> rootGameObjects;
	std::shared_ptr<Hut> hutTemplate;
	int numInstances;

private:
	std::vector<Mesh> hutMeshes;
	std::vector<Mesh> roofMeshes;
	glm::vec3 pos = {0, 0, 0,}, rot = {1, 0, 0}, scale = { 0.1f, 0.1f, 0.1f };
	
	int instancesInRow;
	
};
