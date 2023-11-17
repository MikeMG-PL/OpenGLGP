#include "Engine/GameInstance.h"
#include "Engine/Component.h"
#include "Engine/GameObject.h"

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
	allGameObjects.emplace_back(obj);
}

void GameInstance::UpdateGame()
{

	for (const auto& gameObjectPtr : allGameObjects)
	{
		auto allComponents = gameObjectPtr->GetComponents();

		// Camera handling (moved to rendering loop)

		for (const auto& componentPtr : allComponents)
		{
			componentPtr.get()->Update();
		}
	}
}

void GameInstance::UpdateGameFixed()
{
	for (const auto& gameObjectPtr : allGameObjects)
	{
		for (const auto& componentPtr : gameObjectPtr->GetComponents())
		{
			componentPtr.get()->FixedUpdate();
		}
	}
}

void GameInstance::Count()
{
	// Delta time
	// LAST = NOW;
	// NOW = SDL_GetPerformanceCounter();
	//
	// deltaTime = (NOW - LAST) * 1000 / static_cast<float>(SDL_GetPerformanceFrequency()); // Convert to seconds for more natural movement speeds
}

float GameInstance::GetDeltaTime() const
{
	return deltaTime;
}

void GameInstance::close()
{
	// Close, final cleanup
}