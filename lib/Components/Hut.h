#pragma once
#include <vector>

#include "glm/glm.hpp"
#include "Engine/Component.h"

class Hut : public Component
{
public:

	Hut(const glm::vec3& position, int numHuts, glm::mat4* matrices);

	void Start() override;
	void Update() override;

	std::shared_ptr<GameObject> hutPtr;
	std::shared_ptr<GameObject> roofPtr;

private:

	glm::mat4* matrices;
	int numHuts;
	glm::vec3 position;
};
