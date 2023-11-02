#pragma once
#include <atomic>
#include <memory>

#include "render/Window.h"
#include "render/Listeners/WindowListener.h"

#include "Objects/Object.h"

namespace game
{

class Game : public WindowListener
{
public:
	void createInitParticles();
	void createParticles(int amount);
	void randPosition(Object& obj);

	Game();
	~Game();

	void run();

	// WindowListener implementation
	void onKeyPressed(int key) override;
	void onKeyReleased(int key) override;
	void onExit() override;

private:
	std::shared_ptr<Window> m_window;

	std::vector<Object> m_objects;

	std::atomic<bool> m_running;
};

}