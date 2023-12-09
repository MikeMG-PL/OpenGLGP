#include "Components/Hut.h"

#include "Components/Model.h"
#include "Engine/GameObject.h"

void Hut::Start()
{
	Component::Start();

	auto roof = GameObject::CreateObject();
	roof->AddComponent<Model>("../../res/models/hut/roof.dae");
	roof->GetTransform()->localScale = { 0.1f, 0.1f, 0.1f };
	roof->GetTransform()->localPosition = { 0, 0, -3 };
	roof->GetTransform()->localEulerAngles = { -90, 0, 0 };

	auto hut = GameObject::CreateObject();
	hut->AddComponent<Model>("../../res/models/hut/hut.dae");
	hut->GetTransform()->localScale = { 0.1f, 0.1f, 0.1f };
	hut->GetTransform()->localPosition = { 0, -1, -4 };

	hutPtr = hut;
	roofPtr = roof;
}
