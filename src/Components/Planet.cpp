#include "Components/Planet.h"

#include "Components/Orbit.h"
#include "Engine/GameInstance.h"
#include "Engine/GameObject.h"

void Planet::Start()
{
	Component::Start();
	transform = GetParent()->GetTransform();
}

void Planet::Update()
{
	Component::Update();
	transform->localEulerAngles.y += rotationSpeed * GameInstance::Get().GetDeltaTime();
}
