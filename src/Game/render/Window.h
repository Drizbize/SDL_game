#pragma once
#include "opengl/OpenGl.h"
#include "Objects/Object.h"

#include <memory>
#include <vector>

#include "Render/OpenGL/Renderer.h"

namespace game
{
	
class Object;
class WindowListener;

class Window
{
public:
	Window(int id, WindowListener* listener);
	~Window();

	void run();
	void renderObjects(const std::vector<Object>& objects);
	void swapWindow();
	void update();

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
	std::unique_ptr<OpenGl> m_opengl;

	WindowListener* m_listener;
	std::unique_ptr<Renderer> m_renderer;
	int m_id;
	int m_width;
	int m_height;

	bool m_running;
};

}