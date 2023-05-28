#pragma once
#include "render/Window.h"
#include <memory>

namespace game
{

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	std::shared_ptr<Window> m_window;
};

}