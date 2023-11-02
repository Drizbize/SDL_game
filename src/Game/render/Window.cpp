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

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

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
	, m_width(960)
	, m_height(540)
{
	SdlManager::init();

	m_impl = std::make_unique<Impl>();
	m_impl->m_window = SdlManager::createWindow("Game window", m_width, m_height);
	m_impl->m_context = SdlManager::createContext(m_impl->m_window);
	SDL_GL_MakeCurrent(m_impl->m_window, m_impl->m_context);
	//SDL_GL_SetSwapInterval(1); // Enable vsync

	SdlManager::setAttributes();
	SdlManager::loadOpenGl();
	
	m_renderer = std::make_unique<Renderer>("Resources\\shaders\\Basic.shader");
	m_renderer->init(960.f, 540.f);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(m_impl->m_window, m_impl->m_context);
	ImGui_ImplOpenGL3_Init("#version 330");
}

Window::~Window()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyWindow(m_impl->m_window);
	SDL_Quit();
}

void Window::run()
{
	m_running = true;
}

void Window::renderObjects(const std::vector<Object>& objects)
{
	glClearColor(0.4f, 0.4f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& obj : objects)
	{
		if (obj.shouldRender)
		{
			m_renderer->addObject(obj.pos.x, obj.pos.y);
		}
	}

	m_renderer->draw();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	/*
	static bool show_demo_window = true;
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
	*/
	
	ImGui::Begin("Debug window");
	static float f = 1.f;
	if (ImGui::SliderFloat("zoom", &f, 0.2f, 2.0f))
	{
		m_renderer->zoom(f);
	}
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
		ImGui_ImplSDL2_ProcessEvent(&event);
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
		case SDL_MOUSEMOTION:
			SDL_MouseMotionEvent mouse = event.motion;
			if (mouse.state == SDL_BUTTON_RMASK)
			{
				m_renderer->moveCamera(mouse.xrel, mouse.yrel);
			}
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				SDL_WindowEvent window = event.window;
				m_renderer->resize(window.data1, window.data2);
			}
			break;
		}
	}
}

}