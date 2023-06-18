#include "Window.h"
#include "SDL.h"
#include "glad/glad.h"
#include "Listeners/WindowListener.h"
#include "Game.h"
#include <iostream>

#include "Utils/Utils.h"

enum MoveKeys
{
	Up = SDL_SCANCODE_W,
	Down = SDL_SCANCODE_S,
	Left = SDL_SCANCODE_A,
	Right = SDL_SCANCODE_D
};



struct Object
{
	float posX;
	float posY;
};


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
	, m_width(800)
	, m_height(600)
{
	m_opengl = std::make_unique<OpenGl>(m_width, m_height);

	// this window has a strong SDL && OpenGL dependancy,
	// it is better to have a generic one so we can update it using other API
	// TODO: change Window into interface
	m_impl = std::make_unique<Impl>();

	SDL_Init(SDL_INIT_VIDEO);

	m_impl->m_window = SDL_CreateWindow(
		"Game window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_width,
		m_height,
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

	Object player;
	player.posX = 0.0f;
	player.posY = 0.0f;

	Object player2;
	player2.posX = 10.0f;
	player2.posY = 30.0f;

	SDL_Event event;
	bool running = true;

	m_opengl->initPlayerOpengl("player");
	m_opengl->initPlayerOpengl("player2");

	m_opengl->setColor("player", MYColors::Red);
	m_opengl->setColor("player2", MYColors::Green);

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
				// change SDL_KEY -> KEY
				
				//m_listener->onKeyPressed(event.key.keysym.scancode);

				switch (event.key.keysym.scancode)
				{
				case MoveKeys::Up:
					player.posY += 5;
					break;
				case MoveKeys::Down:
					player.posY -= 5;
					break;
				case MoveKeys::Left:
					player.posX -= 5;
					break;
				case MoveKeys::Right:
					player.posX += 5;
					break;
				case SDL_SCANCODE_Q:
					player.width += 10;
					player.height += 10;
					break;
				case SDL_SCANCODE_R:
					player.width -= 10;
					player.height -= 10;
					break;
				case SDL_SCANCODE_F:
					Object ob;
					ob.posX = rand() % 800 - 400;
					ob.posY = rand() % 800 - 400;
					std::string name = "kavo" + std::to_string(m_objects.size());
					m_objects.push_back(ob);
					m_opengl->initPlayerOpengl(name);
					m_opengl->setColor(name, MYColors::Blue);
					break;
				}
			}
			case SDL_KEYUP:
				//m_listener->onKeyReleased(event.key.keysym.scancode);
				break;

			case SDL_TEXTINPUT:
				break;
			}
		}

		m_opengl->updatePlayerVetrtices("player", player.posX, player.posY, player.width, player.height);
		m_opengl->updatePlayerVetrtices("player2", player2.posX, player2.posY, player2.width, player2.height);
		

		glClearColor(.2f, .6f, .2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		int PlrPosX = (int)player.posX;
		int PlrPosY = (int)player.posY;
		std::cout << "Draw " << "X: " << PlrPosX << " Y: " << PlrPosY << "\n";

		m_opengl->drawShader("player");
		m_opengl->drawShader("player2");

		int x = 0;
		for (auto obj : m_objects)
		{
			std::string name = "kavo" + std::to_string(x);
			x++;
			m_opengl->updatePlayerVetrtices(name, obj.posX, obj.posY, obj.width, obj.height);
			m_opengl->drawShader(name);
		}

		SDL_GL_SwapWindow(m_impl->m_window);
	}
}

}