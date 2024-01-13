#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

#include "Camera.h"
#include "Engine/Component.h"

class Transform : public Component, public std::enable_shared_from_this<Transform>
{

public:

	const std::vector<std::shared_ptr<Transform>>& GetChildren() const; // Is const a problem here?
	void RemoveLastChild();
	glm::mat4 GetLocalModelMatrix() const;
	void UpdateSelfAndChild();
	void AddChild(const std::shared_ptr<Transform>& child);
	glm::vec3 GetPosParentSpace() const;
	glm::vec3 GetRotParentSpace() const;
	glm::vec3 GetScaleParentSpace() const;

	glm::vec3 localPosition = { 0.0f, 0.0f, 0.0f };
	glm::vec3 localEulerAngles = { 0.0f, 0.0f, 0.0f };
	glm::vec3 localScale = { 1.0f, 1.0f, 1.0f };

	// Parent space
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	// This is NOT the GameObject this transform is attached to, but the parent in terms of SceneGraph
	std::shared_ptr<Transform> parentNode;

private:

	std::vector<std::shared_ptr<Transform>> children;
};

