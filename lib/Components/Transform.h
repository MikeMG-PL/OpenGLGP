#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Engine/Component.h"

class Transform : public Component, public std::enable_shared_from_this<Transform>
{

public:

	const glm::mat4& getModelMatrix() const;
	void computeModelMatrix();
	void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix);

	void setLocalPosition(const glm::vec3& newPosition);
	void setLocalEulerAngles(const glm::vec3& newEuler);
	void setLocalScale(const glm::vec3& newScale);

	const glm::vec3& getLocalPosition() const;
	const glm::vec3& getLocalEulerAngles() const;
	const glm::vec3& getLocalScale() const;
	bool IsDirty() const;

	const std::vector<std::shared_ptr<Transform>>& GetChildren() const; // Is const a problem here?
	glm::mat4 GetLocalModelMatrix() const;
	void UpdateSelfAndChild();
	void ForceUpdateSelfAndChild();
	void AddChild(const std::shared_ptr<Transform>& child);

protected:

	bool isDirty = true;

	glm::vec3 localPosition = { 0.0f, 0.0f, 0.0f };
	glm::vec3 localEulerAngles = { 0.0f, 0.0f, 0.0f };
	glm::vec3 localScale = { 1.0f, 1.0f, 1.0f };

	// Global space
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	// This is NOT the GameObject this transform is attached to, but the parent in terms of SceneGraph
	std::shared_ptr<Transform> parentNode;

private:

	std::vector<std::shared_ptr<Transform>> children;
};

