#include "Components/Transform.h"

#include <memory>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "glm/glm.hpp"

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
	if (parentNode)
		modelMatrix = parentNode->modelMatrix * GetLocalModelMatrix();
	else
		modelMatrix = GetLocalModelMatrix();

	for (auto& child : children)
	{
		child->UpdateSelfAndChild();
	}
}

void Transform::AddChild(const std::shared_ptr<Transform>& child)
{
	child->parentNode = shared_from_this();
	children.emplace_back(child);
}

glm::vec3 Transform::GetPosParentSpace() const
{
	if (!parentNode)
		return localPosition;

	glm::vec3 scl;
	glm::quat quat;
	glm::vec3 trans;
	glm::vec3 skew;
	glm::vec4 persp;
	glm::decompose(modelMatrix, scl, quat, trans, skew, persp);
	return trans;
}

glm::vec3 Transform::GetRotParentSpace() const
{
	if (!parentNode)
		return localEulerAngles;

	glm::vec3 scl;
	glm::quat quat;
	glm::vec3 trans;
	glm::vec3 skew;
	glm::vec4 persp;
	glm::decompose(modelMatrix, scl, quat, trans, skew, persp);
	return glm::eulerAngles(quat);
}

glm::vec3 Transform::GetScaleParentSpace() const
{
	if (!parentNode)
		return localScale;

	glm::vec3 scl;
	glm::quat quat;
	glm::vec3 trans;
	glm::vec3 skew;
	glm::vec4 persp;
	glm::decompose(modelMatrix, scl, quat, trans, skew, persp);
	return scl;
}

