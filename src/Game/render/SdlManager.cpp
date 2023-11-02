#include "SdlManager.h"

#include "glad/glad.h"
#include <stdio.h>

namespace game
{
	SdlManager& SdlManager::getInstance()
	{
		static SdlManager manager;
		return manager;
	}

	bool SdlManager::init()
	{
		SdlManager& manager = getInstance();

		if (manager.m_init)
			return false;

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
		{
			printf("Failed to Init SDL: %s\n", SDL_GetError());
			return false;
		}

		manager.m_init = true;
		return true;
	}

	void SdlManager::setAttributes()
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		SDL_GL_SetAttribute(
			SDL_GL_CONTEXT_PROFILE_MASK,
			SDL_GL_CONTEXT_PROFILE_CORE);
	}

	void SdlManager::loadOpenGl()
	{
		gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	}

	SDL_Window* SdlManager::createWindow(const std::string& title, int width, int height)
	{
		return SDL_CreateWindow(
			"Game window",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width,
			height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	}

	SDL_GLContext SdlManager::createContext(SDL_Window* window)
	{
		return SDL_GL_CreateContext(window);
	}
}