#pragma once
#include "render/Window.h"
#include "render/Listeners/WindowListener.h"
#include <memory>

namespace game
{
	struct Object
	{
		float posX;
		float posY;
		float width = 100.f;
		float height = 100.f;
	};

class Game : public WindowListener
{
public:
	Game();
	~Game();

	void run();

	// WindowListener implementation
	void onKeyPressed(int key);
	void onKeyReleased(int key);

	Object player;
private:
	std::shared_ptr<Window> m_window;
};

}