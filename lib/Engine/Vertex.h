#pragma once
#include "glm/glm.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::ivec4 skinIndices;
	glm::vec4 skinWeights;
};
