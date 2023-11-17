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
	void ClearScreen();
	void UpdateScreen();
	void Count();
	float GetDeltaTime() const;

private:

	std::vector<std::shared_ptr<GameObject>> allGameObjects;		// THIS USED TO BE PUBLIC!!!
	// SDL_Renderer* renderer;
	
	// Uint64 NOW = SDL_GetPerformanceCounter();
	// Uint64 LAST = 0;
	float deltaTime = 0;

	bool init(int X, int Y);
	void close();
	GameInstance() = default;

};

