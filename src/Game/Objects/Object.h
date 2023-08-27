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
		Vector2 size{ 5.f, 5.f };

		std::string name;
		bool shouldRender = true;
	};

}