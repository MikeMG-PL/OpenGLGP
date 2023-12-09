#pragma once
#include "glm/glm.hpp"
#include "Engine/Component.h"

class Hut : public Component
{
public:

	Hut(const glm::vec3& position)
		: position(position)
	{
	}

	void Start() override;
	void Update() override;

	std::shared_ptr<GameObject> hutPtr;
	std::shared_ptr<GameObject> roofPtr;

private:

	glm::vec3 position;
};
