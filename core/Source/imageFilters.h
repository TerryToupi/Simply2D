#pragma once

#include <SDL3/SDL.h>
#include <renderer.h>

namespace Simply2D
{
	namespace imageFilters
	{
		bool blur(SDL_Texture* texture, Span<DrawCall> calls, float factor = 1.0f, float bias = 0.0f);
	}
}
