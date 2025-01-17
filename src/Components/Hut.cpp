#include "Components/Hut.h"

#include "Components/Model.h"
#include "Engine/GameInstance.h"
#include "Engine/GameObject.h"

Hut::Hut(const glm::vec3& position, int numHuts, glm::mat4* matrices)
{
	this->position = position;
	this->numHuts = numHuts;
	this->matrices = matrices;
}

void Hut::Start()
{
	Component::Start();

	auto hut = GameObject::CreateObject();
	auto hutTransform = hut->GetTransform();
	hut->AddComponent<Model>("../../res/models/hut/hut.dae", true, numHuts, matrices);
	hutTransform->localScale = { 0.1f, 0.1f, 0.1f };
	GetParent()->GetTransform()->localPosition = position;

	auto roof = GameObject::CreateObject();
	auto roofTransform = roof->GetTransform();
	GetParent()->GetTransform()->AddChild(hutTransform);
	GetParent()->GetTransform()->AddChild(roofTransform);

	roof->AddComponent<Model>("../../res/models/hut/roof.dae", true, numHuts, matrices);
	roofTransform->localPosition = glm::vec3(0, 1, 0);
	roofTransform->localScale = { 0.1f, 0.1f, 0.1f };
	roofTransform->localEulerAngles = { -90, 0, 0 };


	
	hutPtr = hut;
	roofPtr = roof;
}

void Hut::Update()
{
	Component::Update();
}
