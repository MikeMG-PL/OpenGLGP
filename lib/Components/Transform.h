#pragma once
#include <glm/glm.hpp>
#include "Engine/Component.h"

class Transform : public Component
{
public:

	glm::vec2 position = { 0.0f, 0.0f };
	float angle = 0.0f;
	glm::vec2 scale = { 1.0f, 1.0f };
};

