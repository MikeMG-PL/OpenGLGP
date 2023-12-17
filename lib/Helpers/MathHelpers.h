#pragma once
#include <imgui.h>
#include <glm/ext/matrix_transform.hpp>
#include "glm/glm.hpp"

inline glm::vec3 eulerToVector(const glm::vec3& euler)
{
	// Convert degrees to radians
	glm::vec3 eulerRad = glm::radians(euler);

	// Create rotation matrices for pitch, yaw, and roll
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), eulerRad.y, glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), eulerRad.x, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), eulerRad.z, glm::vec3(0.0f, 0.0f, 1.0f));

	// Directional vector (assuming the default direction is along -Z)
	glm::vec4 directionalVector = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

	// Rotate the directional vector using the combined rotation matrix
	directionalVector = rotation * directionalVector;

	// Extract the resulting vector from the rotated directional vector
	return glm::vec3(directionalVector);
}

inline glm::vec3 ImVec4ToVec3(const ImVec4& imvec4) {
	return glm::vec3(imvec4.x, imvec4.y, imvec4.z);
}