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
	instancedShader = Renderer::Get().GetInstancedShader();

	auto comps = GameInstance::Get().allComponents;
	for (auto comp : comps)
	{
		if ((camera = std::dynamic_pointer_cast<Camera>(comp)))
			break;
	}

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

			root->GetTransform()->AddChild(walls->GetTransform());
			root->GetTransform()->AddChild(roof->GetTransform());

			root->GetTransform()->localPosition = trans;
			root->GetTransform()->localEulerAngles = glm::eulerAngles(quat);
			root->GetTransform()->localScale = scl;

			roof->GetTransform()->localPosition = initialRoofTranslation;
			roof->GetTransform()->localEulerAngles = { -90.0f, 0, 0 };

			rootGameObjects.emplace_back(root);
		}
	}

	for (int n = 0; n < rootGameObjects.size(); n++)
	{
		UpdateTransforms(n);
	}

	matrices = wallMatrices;

	auto hut = GameObject::CreateObject();
	hutTemplate = hut->AddComponent<Hut>(pos, numInstances, matrices);

	hutMeshes = hutTemplate->hutPtr->GetComponent<Model>()->GetMeshes();
	roofMeshes = hutTemplate->roofPtr->GetComponent<Model>()->GetMeshes();

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);
}

void HutSpawner::RenderUpdate()
{
	RenderInjector::RenderUpdate();

	instancedShader.use();
	instancedShader.setMat4("projection", Renderer::Get().GetProjectionMatrix());
	instancedShader.setMat4("view", Renderer::Get().GetViewMatrix());
	instancedShader.setVector3("viewPos", camera->GetParent()->GetTransform()->localPosition);
	
	Draw(instancedShader, 0); // this 0 is for historical reasons
}

void HutSpawner::Draw(Shader shader, int instanceID)
{
	for (int i = 0; i < hutMeshes.size(); i++)
	{
		matrices = wallMatrices;
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);
		hutMeshes[i].SetupInstancing();
		hutMeshes[i].Draw(shader, instanceID);
		matrices = roofMatrices;
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);
		roofMeshes[i].SetupInstancing();
		roofMeshes[i].Draw(shader, instanceID);
	}
}

void HutSpawner::UpdateTransforms(int n)
{
	glm::mat4 model = glm::mat4(1.0f);

	auto root = rootGameObjects[n];
	auto walls = rootGameObjects[n]->GetTransform()->GetChildren()[0];
	auto roof = rootGameObjects[n]->GetTransform()->GetChildren()[1];

	model = rootGameObjects[n]->GetTransform()->modelMatrix;
	rootMatrices[n] = model;
	root->GetTransform()->UpdateSelfAndChild();

	model = root->GetTransform()->modelMatrix * walls->GetLocalModelMatrix(); // modelMatrix
	wallMatrices[n] = model;
	root->GetTransform()->UpdateSelfAndChild();

	model = root->GetTransform()->modelMatrix * roof->GetLocalModelMatrix(); // modelMatrix
	roofMatrices[n] = model;
	root->GetTransform()->UpdateSelfAndChild();

}
