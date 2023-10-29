#pragma once

namespace game
{

struct WindowListener
{
	virtual void onKeyPressed(int key) = 0;
	virtual void onKeyReleased(int key) = 0;
	virtual void onExit() = 0;
};
}