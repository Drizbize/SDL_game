#include "Game.h"
#include "SDL.h"
#include "glad/glad.h"
#include <iostream>

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
	createInitParticles();

	m_running = true;
	m_window->run();

	//gameloop
	while (m_running)
	{
		m_window->update();

		m_window->renderObjects(m_objects);
		m_window->swapWindow();
	}

	m_window.reset();
}
void Game::onKeyPressed(int key)
{
	printf("Key pressed: %d", key);
}

void Game::onKeyReleased(int key)
{
}

void Game::onExit()
{
	m_running = false;
}

void Game::createInitParticles()
{
	createParticles("Red", 325, MYColors::Red);
	createParticles("Blue", 325, MYColors::Blue);
	createParticles("Green", 325, MYColors::Green);
	createParticles("White", 325, MYColors::White);
	createParticles("Yellow", 325, MYColors::Yellow);
}

void Game::createParticles(std::string name, int amount, game::MYColors color)
{
	Object newObject;
	newObject.name = name;

	for (int i = 0; i < amount; i++)
	{
		Object newObject;
		newObject.name = name + std::to_string(i);
		randPosition(newObject);

		m_objects.push_back(newObject);
	}
}

void Game::randPosition(Object& obj)
{
	obj.pos.x = rand() % (m_window->getWidth() * 2) - m_window->getWidth();
	obj.pos.y = rand() % (m_window->getHeight() * 2) - m_window->getHeight();
}

}