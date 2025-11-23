#pragma once

#include "Types/Arrays.h"

namespace Simply2D
{
	template<typename T>
	int number_sign(T x)
	{
		return x > 0 ? 1 : x < 0 ? -1 : 0;
	}

	using Rect = TArray<int, 4>;
}