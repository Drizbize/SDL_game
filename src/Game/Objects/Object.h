#pragma once
#include <memory>
#include <string>

namespace game
{
	struct Vector2
	{
		float x = 0;
		float y = 0;
	};

	struct Object
	{
		Vector2 pos;
		Vector2 size{ 100.f, 100.f };

		bool shouldRender = true;
	};

}