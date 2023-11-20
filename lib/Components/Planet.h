#pragma once
#include "Transform.h"
#include "Engine/Component.h"

class Planet : public Component
{
public:

	Planet(float rotation_speed, bool root = false)
		: rotationSpeed(rotation_speed), root(root)
	{
	}

	float rotationSpeed = 5;
	void Start() override;
	void Update() override;

private:

	bool randomBool();
	bool root;
	std::shared_ptr<Transform> transform;

};
