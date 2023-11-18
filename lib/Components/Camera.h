#pragma once
#include "Engine/Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera : public Component
{
public:

	glm::vec3 target;
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

	void Update() override;

	glm::mat4 view = glm::mat4(1.0f);

private:

	void cameraLogic();
	glm::vec3 worldUp = glm::vec3(0, 1, 0);

};
