#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Components/Hut.h"
#include "Model.h"
#include "Engine/Component.h"

class HutSpawner : public Component
{
public:

	HutSpawner(int instancesInRow);
	~HutSpawner() override;
	void Start() override;
	void Draw(Shader shader, int instanceID);

	int hutOffset = 3;
	glm::mat4* matrices;
	std::shared_ptr<Hut> hutTemplate;

private:

	std::vector<Mesh> hutMeshes;
	std::vector<Mesh> roofMeshes;

	glm::vec3 pos = {0, 0, 0,}, rot = {1, 1, 1}, scale = { 0.1f, 0.1f, 0.1f };
	int numInstances;
	int instancesInRow;
	
};
