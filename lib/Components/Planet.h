#pragma once
#include "Transform.h"
#include "Engine/Component.h"

class Planet : public Component
{
public:

	Planet(float rotation_speed)
		: rotationSpeed(rotation_speed)
	{
	}

	float rotationSpeed = 5;
	void Start() override;
	void Update() override;

private:

	std::shared_ptr<Transform> transform;

};
