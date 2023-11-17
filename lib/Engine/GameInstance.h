#pragma once
#include <memory>
#include <vector>
#include <glm/vec2.hpp>
class GameObject;

class GameInstance
{
public:

	GameInstance& operator=(const GameInstance&) = delete;
	GameInstance(const GameInstance&) = delete;
	~GameInstance();

	static GameInstance& Get();
	bool StartGame(int windowX, int windowY);
	void RegisterObject(std::shared_ptr<GameObject> const& obj);
	void UpdateGame();
	void UpdateGameFixed();
	void ClearScreen();
	void UpdateScreen();
	void Count();
	float GetDeltaTime() const;

	// SDL_Renderer* GetRenderer() const;
	std::vector<std::shared_ptr<GameObject>> allGameObjects;

private:

	int windowX = 0, windowY = 0;
	int cameraPosX = 0, cameraPosY = 0;
	// SDL_Renderer* renderer;
	// SDL_Window* window;
	// Uint64 NOW = SDL_GetPerformanceCounter();
	// Uint64 LAST = 0;
	float deltaTime = 0;

	bool init(int X, int Y);
	void close();
	GameInstance() = default;

};

