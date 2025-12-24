#pragma once

#include "Types/Arrays.h"

namespace Simply2D
{
	template<typename T>
	int number_sign(T x)
	{
		return x > 0 ? 1 : x < 0 ? -1 : 0;
	}

	//using Rect = TArray<int, 4>;
	//using Triagle = TArray<int, 3>;

	struct Rect 
	{
		uint32_t x = 0;
		uint32_t y = 0;
		uint32_t w = 0;
		uint32_t h = 0;

		Rect() = default;

		Rect(uint32_t _x, uint32_t _y, uint32_t _w, uint32_t _h)
			:	x(_x), y(_y), w(_w), h(_h) { }

		Rect(uint32_t _w, uint32_t _h)
			:	x(0), y(0), w(_w), h(_h) { }
	};
}