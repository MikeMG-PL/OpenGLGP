#pragma once
#include <memory>
#include <vector>

#include "Component.h"
#include "glad/glad.h"

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
	float GetDeltaTime() const; // In seconds

	// Public for convenience
	std::vector<std::shared_ptr<GameObject>> allGameObjects = {};
	std::vector<std::shared_ptr<Component>> allComponents = {};
	std::vector<std::shared_ptr<GameObject>> test = {};

private:
	
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	void close();
	GameInstance() = default;

};

