#include "Window.h"

#include <ctime>
#include <iostream>
#include <math.h>
#include <vector>

#include "glad/glad.h"

#include "imgui/imgui.h"

#include "Game.h"
#include "SdlManager.h"

#include "Listeners/WindowListener.h"
#include "Utils/Utils.h"

namespace game
{
	struct Window::Impl
	{
		SDL_Window* m_window;
		SDL_GLContext m_context;
	};


Window::Window(int id, WindowListener* listener)
	: m_id(id)
	, m_listener(listener)
	, m_width(1000)
	, m_height(700)
{
	m_opengl = std::make_unique<OpenGl>(m_width, m_height);

	SdlManager::init();

	m_impl = std::make_unique<Impl>();
	m_impl->m_window = SdlManager::createWindow("Game window", m_width, m_height);
	m_impl->m_context = SdlManager::createContext(m_impl->m_window);

	SdlManager::setAttributes();
	SdlManager::loadOpenGl();
	
	m_renderer = std::make_unique<Renderer>("Resources\\shaders\\Basic.shader");
	m_renderer->init();
}

Window::~Window()
{
	SDL_DestroyWindow(m_impl->m_window);
	SDL_Quit();
}

void Window::run()
{
	m_running = true;
}

void Window::renderObjects(const std::vector<Object>& objects)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& obj : objects)
	{
		if (obj.shouldRender)
		{
			m_renderer->addObject(obj.pos.x, obj.pos.y);
		}
	}

	m_renderer->draw();
}

void Window::swapWindow()
{
	SDL_GL_SwapWindow(m_impl->m_window);
}

void Window::update()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_running = false;
			m_listener->onExit();
			break;
		case SDL_KEYDOWN:
			SDL_KeyboardEvent key = event.key;
			m_listener->onKeyPressed(event.key.keysym.scancode);
			break;
		}
	}
}

}