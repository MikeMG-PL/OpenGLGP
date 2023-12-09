#pragma once
#include "Engine/Component.h"

class Hut : public Component
{
public:

	void Start() override;

	std::shared_ptr<GameObject> hutPtr;
	std::shared_ptr<GameObject> roofPtr;
};
