#include "Components/HutSpawner.h"

#include <memory>
#include <memory>
#include <memory>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Components/Hut.h"
#include "Components/Model.h"
#include "Engine/GameObject.h"
#include "Engine/Renderer.h"
#include "GLFW/glfw3.h"

HutSpawner::HutSpawner(int instancesInRow)
{
	this->instancesInRow = instancesInRow;
	this->numInstances = instancesInRow * instancesInRow;
	matrices = new glm::mat4[numInstances];
	roofMatrices = new glm::mat4[numInstances];
	wallMatrices = new glm::mat4[numInstances];
	rootMatrices = new glm::mat4[numInstances];
}

HutSpawner::~HutSpawner()
{
	delete[] matrices;
	delete[] wallMatrices;
	delete[] roofMatrices;
	delete[] rootMatrices;
}

void HutSpawner::Start()
{
	Component::Start();
	glm::vec3 scl;
	glm::quat quat;
	glm::vec3 trans;
	glm::vec3 skew;
	glm::vec4 persp;
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

			auto root = GameObject::CreateObject(false);
			auto walls = GameObject::CreateObject(false);
			auto roof = GameObject::CreateObject(false);

			rootMatrices[n] = glm::mat4(1.0f);
			wallMatrices[n] = glm::mat4(1.0f);
			roofMatrices[n] = glm::mat4(1.0f);

			rootMatrices[n] = model;
			glm::decompose(rootMatrices[n], scl, quat, trans, skew, persp);
			root->GetTransform()->setLocalPosition(trans);
			root->GetTransform()->setLocalEulerAngles(glm::eulerAngles(quat));
			root->GetTransform()->setLocalScale(scl);

			root->GetTransform()->AddChild(walls->GetTransform());
			root->GetTransform()->AddChild(roof->GetTransform());

			root->GetTransform()->ForceUpdateSelfAndChild();

			roof->GetTransform()->setLocalPosition(initialRoofTranslation);
			roof->GetTransform()->setLocalEulerAngles({-90.0f, 0, 0});

			root->GetTransform()->ForceUpdateSelfAndChild();

			std::cout << root->GetTransform()->getLocalPosition().x << std::endl;

			wallMatrices[n] = model;

			// model = glm::mat4(1.0f);
			// model = glm::translate(model, roof->GetTransform()->getLocalPosition());
			// model = glm::scale(model, roof->GetTransform()->getLocalScale());
			
			roofMatrices[n] = model;

			// glm::decompose(rootMatrices[n], scl, quat, trans, skew, persp);

			// auto root = GameObject::CreateObject(false);
			// auto walls = GameObject::CreateObject(false);
			// auto roof = GameObject::CreateObject(false);
			// root->GetTransform()->AddChild(walls->GetTransform());
			// root->GetTransform()->AddChild(roof->GetTransform());
			// root->GetTransform()->setLocalPosition(pos);
			// root->GetTransform()->setLocalScale({ 0.1f, 0.1f, 0.1f });
			//
			// roof->GetTransform()->setLocalPosition(initialRoofTranslation);
			// roof->GetTransform()->setLocalEulerAngles({ -90.0f, 0, 0 });
			//
			rootGameObjects.emplace_back(root);
			//
			// rootMatrices[n] = root->GetTransform()->getModelMatrix();
			// wallMatrices[n] = root->GetTransform()->GetChildren()[0]->getModelMatrix();
			// roofMatrices[n] = root->GetTransform()->GetChildren()[1]->getModelMatrix();
		}
	}

	//UpdateTransforms();
	matrices = wallMatrices;

	auto hut = GameObject::CreateObject();
	hutTemplate = hut->AddComponent<Hut>(pos, numInstances, matrices);

	hutMeshes = hutTemplate->hutPtr->GetComponent<Model>()->GetMeshes();
	roofMeshes = hutTemplate->roofPtr->GetComponent<Model>()->GetMeshes();

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);
}

void HutSpawner::Draw(Shader shader, int instanceID)
{
	for (int i = 0; i < hutMeshes.size(); i++)
	{
		matrices = wallMatrices;
		glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);
		hutMeshes[i].SetupInstancing();
		hutMeshes[i].Draw(shader, instanceID);
		matrices = roofMatrices;
		glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);
		roofMeshes[i].SetupInstancing();
		roofMeshes[i].Draw(shader, instanceID);
	}
}

void HutSpawner::UpdateTransforms()
{
	for (int i = 0; i < rootGameObjects.size(); i++)
	{
		rootMatrices[i] = rootGameObjects[i]->GetTransform()->getModelMatrix();
		wallMatrices[i] = rootGameObjects[i]->GetTransform()->GetChildren()[0]->getModelMatrix();
		roofMatrices[i] = rootGameObjects[i]->GetTransform()->GetChildren()[1]->getModelMatrix();
	}
}
