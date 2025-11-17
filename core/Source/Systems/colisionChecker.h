#pragma once

#include <deque>
#include "Scene/sprite.h"

namespace Simply2D
{
	namespace ColisionChecker
	{
		void check(std::deque<Sprite*>& sprites);
	}
}
