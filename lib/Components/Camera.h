#pragma once
#include "Engine/Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMode
{
	FLYING,
	CAR
};

class Camera : public Component, public std::enable_shared_from_this<Camera>
{
public:

	glm::vec3 target;
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

	void Start() override;
	void Update() override;
	glm::vec3 GetWorldUp() const;
	glm::vec3 GetWorldFront() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetFront() const;

	glm::mat4 view = glm::mat4(1.0f);

	CameraMode cameraMode = FLYING;

private:

	glm::vec3 eulerToDirectionVector(glm::vec3 euler);
	void cameraLogic();
	glm::vec3 worldUp = glm::vec3(0, 1, 0);
	glm::vec3 worldFront = glm::vec3(0, 0, -1);

};
