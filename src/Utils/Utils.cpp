#include "Utils.h"

namespace Utils
{
	void clamp(float& inValue, float left, float right)
	{
		if (inValue < left)
			inValue = left;

		if (inValue > right)
			inValue = right;
	}
}