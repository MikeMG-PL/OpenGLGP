#include "Engine/GameInstance.h"

#include <memory>

#include "Engine/Component.h"
#include "Engine/GameObject.h"
#include "GLFW/glfw3.h"

GameInstance::~GameInstance()
{
	close();
}

GameInstance& GameInstance::Get()
{
	static GameInstance instance;
	return instance;
}

void GameInstance::StartGame()
{
	return;
}

void GameInstance::RegisterObject(std::shared_ptr<GameObject> const& obj)
{
	allGameObjects.push_back(obj);
}

void GameInstance::UpdateGame()
{
	for (const auto& gameObjectPtr : allGameObjects)
	{
		if(!gameObjectPtr->allowUpdate)
			return;

		auto components = gameObjectPtr->GetComponents();
		
		for (const auto& componentPtr : components)
		{
			componentPtr.get()->Update();
		}
	}
}

void GameInstance::UpdateGameFixed()
{
	for (const auto& gameObjectPtr : allGameObjects)
	{
		if (!gameObjectPtr->allowUpdate)
			return;

		for (const auto& componentPtr : gameObjectPtr->GetComponents())
		{
			componentPtr.get()->FixedUpdate();
		}
	}
}

void GameInstance::Count()
{
	const GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

float GameInstance::GetDeltaTime() const
{
	return deltaTime;
}

void GameInstance::close()
{
	// Close, final cleanup
}