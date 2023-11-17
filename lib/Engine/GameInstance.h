#pragma once
#include <memory>
#include <vector>
#include <glm/vec2.hpp>
// #include "GLFW/glfw3.h"

class GameObject;
struct GLFWwindow;

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
	glm::vec2 GetWindowSize() const;
	GLFWwindow* GetWindow() const;
	const char* GetGLSLVersion() const;
	// SDL_Renderer* GetRenderer() const;

	// Helper OpenGL helper functions
	static void glfw_error_callback(int error, const char* description);
	bool ShouldCloseWindow();
	void CloseWindow();

private:

	std::vector<std::shared_ptr<GameObject>> allGameObjects;		// THIS USED TO BE PUBLIC!!!
	int windowX = 0, windowY = 0;
	int cameraPosX = 0, cameraPosY = 0;
	GLFWwindow* window;
	const char* glsl_version;
	// SDL_Renderer* renderer;
	
	// Uint64 NOW = SDL_GetPerformanceCounter();
	// Uint64 LAST = 0;
	float deltaTime = 0;

	bool init(int X, int Y);
	void close();
	GameInstance() = default;

};

