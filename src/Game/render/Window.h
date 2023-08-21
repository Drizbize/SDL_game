#pragma once
#include "opengl/OpenGl.h"
#include <memory>
#include <vector>

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
private:	
	struct Impl;
	std::unique_ptr<Impl> m_impl;

	std::unique_ptr<OpenGl> m_opengl;

	WindowListener* m_listener;
	int m_id;

	int m_width;
	int m_height;

	std::vector<Object> m_objects;
};

}