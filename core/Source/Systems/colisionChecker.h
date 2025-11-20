#pragma once

#include "Scene/sprite.h"
#include "Types/Deque.h"

namespace Simply2D
{
	namespace ColisionChecker
	{
		void check(TDeque<Sprite*>& sprites);
	}
}
