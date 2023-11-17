#pragma once
#include <memory>
#include <vector>

class GameObject;
struct GLFWwindow;

class GameInstance
{
public:

	GameInstance& operator=(const GameInstance&) = delete;
	GameInstance(const GameInstance&) = delete;
	~GameInstance();

	static GameInstance& Get();
	void StartGame();
	void RegisterObject(std::shared_ptr<GameObject> const& obj);
	void UpdateGame();
	void UpdateGameFixed();
	void Count();
	float GetDeltaTime() const;

	// Public for convenience
	std::vector<std::shared_ptr<GameObject>> allGameObjects;

private:
	
	// Uint64 NOW = SDL_GetPerformanceCounter();
	// Uint64 LAST = 0;
	float deltaTime = 0;

	void close();
	GameInstance() = default;

};

