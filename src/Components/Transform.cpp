#include "Components/Transform.h"

#include <memory>
#include <glm/ext/matrix_transform.hpp>
#include "glm/glm.hpp"

const glm::mat4& Transform::getModelMatrix() const
{
	return modelMatrix;
}

void Transform::computeModelMatrix()
{
	modelMatrix = GetLocalModelMatrix();
	isDirty = false;
}

void Transform::computeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
{
	modelMatrix = parentGlobalModelMatrix * GetLocalModelMatrix();
	isDirty = false;
}

void Transform::setLocalPosition(const glm::vec3& newPosition)
{
	localPosition = newPosition;
	isDirty = true;
}

void Transform::setLocalEulerAngles(const glm::vec3& newEuler)
{
	localEulerAngles = newEuler;
	isDirty = true;
}

void Transform::setLocalScale(const glm::vec3& newScale)
{
	localScale = newScale;
	isDirty = true;
}

const glm::vec3& Transform::getLocalPosition() const
{
	return localPosition;
}

const glm::vec3& Transform::getLocalEulerAngles() const
{
	return localEulerAngles;
}

const glm::vec3& Transform::getLocalScale() const
{
	return localScale;
}

bool Transform::IsDirty() const
{
	return isDirty;
}

const std::vector<std::shared_ptr<Transform>>& Transform::GetChildren() const
{
	return children;
}

glm::mat4 Transform::GetLocalModelMatrix() const
{
	const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
		glm::radians(localEulerAngles.x),
		glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
		glm::radians(localEulerAngles.y),
		glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
		glm::radians(localEulerAngles.z),
		glm::vec3(0.0f, 0.0f, 1.0f));

	// Z * Y * X
	const glm::mat4 rotationMatrix = transformZ * transformY * transformX;

	// translation * rotation * scale (also know as TRS matrix)
	return glm::translate(glm::mat4(1.0f), localPosition) *
		rotationMatrix *
		glm::scale(glm::mat4(1.0f), localScale);
}

void Transform::UpdateSelfAndChild()
{
	if (IsDirty())
	{
		ForceUpdateSelfAndChild();
		return;
	}

	for (auto&& child : children)
	{
		child->UpdateSelfAndChild();
	}
}

void Transform::ForceUpdateSelfAndChild()
{
	if (parentNode)
		computeModelMatrix(parentNode->getModelMatrix());
	else
		computeModelMatrix();

	for (auto&& child : children)
	{
		child->ForceUpdateSelfAndChild();
	}
}

void Transform::AddChild(const std::shared_ptr<Transform>& child)
{
    child->parentNode = shared_from_this();
    children.emplace_back(child);
}

