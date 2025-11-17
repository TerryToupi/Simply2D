#include <pch.h>

#include "Source/Base/gameTime.h"

#include <SDL3/SDL.h>

namespace Simply2D
{
	double Clock::getTime(void) 
	{
		Uint64 counter = SDL_GetPerformanceCounter();
		Uint64 freq = SDL_GetPerformanceFrequency();

		return static_cast<double>(counter) / static_cast<double>(freq);
	}
}