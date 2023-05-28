#include "Window.h"
#include "SDL.h"
#include "glad/glad.h"
#include <iostream>

namespace game
{
	struct Window::Impl
	{
		SDL_Window* m_window;
		SDL_GLContext m_context;
	};


Window::Window(int id)
	: m_id(id)
{
	// this window has a strong SDL && OpenGL dependancy,
	// it is better to have a generic one so we can update it using other API
	// TODO: change Window into interface
	m_impl = std::make_unique<Impl>();

	SDL_Init(SDL_INIT_VIDEO);

	m_impl->m_window = SDL_CreateWindow(
		"Game window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800,
		600,
		SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
	
	m_impl->m_context = SDL_GL_CreateContext(m_impl->m_window);

	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
}

Window::~Window()
{
	SDL_DestroyWindow(m_impl->m_window);
	SDL_Quit();
}

void Window::run()
{
	// this is our window/render loop
	// we should run it in a separate thread, or otherwise
	// nothing else can be executed
	// 
	// TODO: run window thread in a different thread
	SDL_Event event;
	bool running = true;

	while (running)
	{
		// we are checking if there are any new window events...
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			// ... and if this event is "Quit" we stop the window
			case SDL_QUIT:
				running = false;
				break;

			// TODO: add implementation for inputs handling
			case SDL_KEYDOWN:
			{
				SDL_KeyboardEvent key = event.key;
				break;
			}
			case SDL_KEYUP:
				std::cout << "Jump !";
				break;

			case SDL_TEXTINPUT:
				break;
			}
		}

		glClearColor(0.04f, 0.7f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(m_impl->m_window);
	}
}

}