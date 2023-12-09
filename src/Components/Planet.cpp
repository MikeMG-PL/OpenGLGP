#include "Components/Planet.h"

#include "Components/Orbit.h"
#include "Engine/Editor.h"
#include "Engine/GameInstance.h"
#include "Engine/GameObject.h"

void Planet::Start()
{
	Component::Start();
	transform = GetParent()->GetTransform();
	if (randomBool())
		rotationSpeed *= -1;
}

void Planet::Update()
{
	Component::Update();
	transform->localEulerAngles.y += rotationSpeed * GameInstance::Get().GetDeltaTime();

	// if(root)
	// 	transform->localEulerAngles.y = Editor::Get().sunRotation;
}

bool Planet::randomBool()
{
	return 0 + (rand() % (1 - 0 + 1)) == 1;
}
