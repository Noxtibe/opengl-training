#pragma once
#include "Window.h"
#include "Renderer.h"
class Game
{
public:
	Game();
	bool initialize();
	void loop();
	void close();

private:
	void processInput();
	void update();
	void render();

	bool isRunning;
	Window window;
	Renderer renderer;

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
};

