#include "Components/Hut.h"

#include "Components/Model.h"
#include "Engine/GameInstance.h"
#include "Engine/GameObject.h"
#include "GLFW/glfw3.h"

void Hut::Start()
{
	Component::Start();

	auto hut = GameObject::CreateObject();
	hut->AddComponent<Model>("../../res/models/hut/hut.dae");
	hut->GetTransform()->localScale = { 0.1f, 0.1f, 0.1f };
	hut->GetTransform()->localPosition = position;

	auto roof = GameObject::CreateObject();
	hut->GetTransform()->AddChild(roof->GetTransform());
	
	roof->AddComponent<Model>("../../res/models/hut/roof.dae");
	roof->GetTransform()->localPosition = glm::vec3(0, 10, 0);
	roof->GetTransform()->localEulerAngles = { -90, 0, 0 };

	hutPtr = hut;
	roofPtr = roof;
}

void Hut::Update()
{
	Component::Update();
}
