#pragma once
#include <memory>

namespace game
{

class Window
{
public:
	Window(int id);
	~Window();

	void run();

private:
	// we want to hide SDL implementation
	struct Impl;
	std::unique_ptr<Impl> m_impl;

	int m_id;
};

}