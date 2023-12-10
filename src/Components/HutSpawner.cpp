#include "Components/HutSpawner.h"

#include <memory>
#include <glm/ext/matrix_transform.hpp>

#include "Components/Hut.h"
#include "Components/Model.h"
#include "Engine/GameObject.h"
#include "Engine/Renderer.h"

HutSpawner::HutSpawner(int instancesInRow)
{
	this->instancesInRow = instancesInRow;
	this->numInstances = instancesInRow * instancesInRow;
	matrices = new glm::mat4[numInstances];
}

HutSpawner::~HutSpawner()
{
	delete[] matrices;
}

void HutSpawner::Start()
{
	Component::Start();

	for (int i = 0; i < instancesInRow; i++)
	{
		// Check the order of iteration
		for (int j = 0; j < instancesInRow; j++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			pos = glm::vec3(i * hutOffset, 0, j * hutOffset);

			model = glm::translate(model, pos);
			model = glm::scale(model, scale);
			model = glm::rotate(model, 0.0f, rot);

			const int n = i * instancesInRow + j;
			matrices[n] = model;
		}
	}

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);

	auto hut = GameObject::CreateObject();
	hutTemplate = hut->AddComponent<Hut>(pos, numInstances, matrices);

	hutMeshes = hutTemplate->hutPtr->GetComponent<Model>()->GetMeshes();
	roofMeshes = hutTemplate->hutPtr->GetComponent<Model>()->GetMeshes();

	for (int i = 0; i < hutMeshes.size(); i++)
	{
		hutMeshes[i].SetupInstancing();
		roofMeshes[i].SetupInstancing();
	}
}

void HutSpawner::Draw(Shader shader, int instanceID)
{
	for (int i = 0; i < hutMeshes.size(); i++)
	{
		hutMeshes[i].Draw(shader, instanceID);
		roofMeshes[i].Draw(shader, instanceID);
	}
}
