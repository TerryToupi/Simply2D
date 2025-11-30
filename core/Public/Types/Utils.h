#pragma once

#include "EASTL/internal/copy_help.h"

template<typename T>
inline void TSwap(T& a, T& b)
{
	eastl::swap(a, b);
}

template<typename U>
constexpr typename eastl::remove_reference<U>::type&& TMove(U&& value)
{
	return static_cast<typename eastl::remove_reference<U>::type&&>(value);
}

