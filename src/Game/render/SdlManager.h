#pragma once
#include "SDL.h"
#include <string>

namespace game
{

class SdlManager
{
public:
	static SdlManager& getInstance();

	static bool init();
	static void setAttributes();
	static void loadOpenGl();

	static SDL_Window* createWindow(const std::string& title, int width, int height);
	static SDL_GLContext createContext(SDL_Window* window);

private:
	SdlManager() {};

	bool m_init = false;
};

}