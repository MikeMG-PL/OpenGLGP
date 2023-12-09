#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Engine/Component.h"

class HutSpawner : public Component
{
public:

	HutSpawner(unsigned int instancesInRow);
	void Start() override;

	int hutOffset = 3;

private:

	unsigned int numInstances;
	unsigned int instancesInRow;
	std::vector<glm::mat4> matrices;
};
