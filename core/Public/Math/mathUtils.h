#pragma once

#include <array>

namespace Simply2D
{
	template<typename T>
	int number_sign(T x)
	{
		return x > 0 ? 1 : x < 0 ? -1 : 0;
	}

	using Rect = std::array<int, 4>;
}