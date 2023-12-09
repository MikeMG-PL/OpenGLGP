#include "Components/HutSpawner.h"

#include "Components/Hut.h"
#include "Engine/GameObject.h"

HutSpawner::HutSpawner(unsigned int instancesInRow)
{
	this->instancesInRow = instancesInRow;
	this->numInstances = instancesInRow * instancesInRow;
	matrices.resize(numInstances);
}

void HutSpawner::Start()
{
	Component::Start();

	for(int n = 0; n < numInstances; n++)
		matrices[n] = glm::mat4(1.0f);

	for (int i = 0; i < instancesInRow; i++)
	{
		// Check the order of iteration
		for (int j = 0; j < instancesInRow; j++)
		{
			const glm::mat4 model = glm::mat4(1.0f);
			glm::vec3 pos = {}, rot = {}, scale = { 1, 1, 1 };
			pos = glm::vec3(i * hutOffset, 0, j * hutOffset);

			const unsigned int n = i * instancesInRow + j;
			matrices[n] = model;

			auto hut = GameObject::CreateObject();
			hut->AddComponent<Hut>(pos);
		}
	}
}
