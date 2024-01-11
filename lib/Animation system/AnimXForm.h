#pragma once
#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>

struct xform
{
	glm::vec3 position;
	glm::quat rotation;
};