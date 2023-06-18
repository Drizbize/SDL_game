#include "Game.h"
#include "SDL.h"
#include "glad/glad.h"
#include <iostream>

enum MoveKeys
{
	Up = SDL_SCANCODE_W,
	Down = SDL_SCANCODE_S,
	Left = SDL_SCANCODE_A,
	Right = SDL_SCANCODE_D
};

namespace game
{

Game::Game()
{
	m_window = std::make_shared<Window>(0, this);
}

Game::~Game()
{
	m_window.reset();
}

void Game::run()
{
	player.posX = 400;
	player.posY = 300;

	m_window->run();
}
void Game::onKeyPressed(int key)
{
	std::cout << "[KEY PRESSED] " << key << "\n";
	switch (key)
	{
	case MoveKeys::Up:
		player.posY--;
		break;
	case MoveKeys::Down:
		player.posY++;
		break;
	case MoveKeys::Left:
		player.posX--;
		break;
	case MoveKeys::Right:
		player.posX++;
		break;
	}
}

void Game::onKeyReleased(int key)
{
	//int y = 5;
	//(void)y;
}

}