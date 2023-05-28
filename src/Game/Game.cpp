#include "Game.h"
#include "SDL.h"
#include "glad/glad.h"

namespace game
{

Game::Game()
{
	m_window = std::make_shared<Window>(0);
}

Game::~Game()
{
	m_window.reset();
}

void Game::run()
{
	m_window->run();
}
}