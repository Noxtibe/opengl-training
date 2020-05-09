#include "Window.h"
#include "Log.h"

Window::Window() : SDLWindow(nullptr), width(WINDOW_WIDTH), height(WINDOW_HEIGHT)
{
}

bool Window::initialize()
{
	int sdlInitResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlInitResult != 0) {
		Log::error("Unable to initialize SDL");
		return false;
	}
	SDLWindow = SDL_CreateWindow("Wall Pong", 100, 100, width, height, 0); // 0 is flag we will use later
	if (!SDLWindow)
	{
		Log::error("Failed to create window");
		return false;
	}
	return true;
}

void Window::close()
{
	SDL_DestroyWindow(SDLWindow);
}

SDL_Window* Window::getSDLWindow()
{
	return SDLWindow;
}

int Window::getWidth()
{
	return width;
}

int Window::getHeight()
{
	return height;
}
