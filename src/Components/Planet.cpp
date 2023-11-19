#include "Components/Planet.h"

#include "Components/Orbit.h"
#include "Engine/GameInstance.h"
#include "Engine/GameObject.h"

void Planet::Start()
{
	Component::Start();
	transform = GetParent()->GetTransform();
	// GetParent()->AddComponent<Orbit>(0, 0, 5, 5, 32);
}

void Planet::Update()
{
	Component::Update();
	transform->localEulerAngles.y += rotationSpeed * GameInstance::Get().GetDeltaTime();
}
