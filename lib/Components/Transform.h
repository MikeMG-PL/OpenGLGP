#pragma once
#include <glm/glm.hpp>
#include "Engine/Component.h"

class Transform : public Component
{

public:

	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 eulerAngles = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
};

