#pragma once

#include "Memory/memory.h"
#include "EASTL/vector.h"
#include "EASTL/fixed_vector.h"
#include "EASTL/array.h"

template<typename T>
using TVector = eastl::vector<T>;

template<typename T, eastl_size_t S>
using TInlineVector = eastl::fixed_vector<T, S, true>;

template<typename T, eastl_size_t S> 
using TArray = eastl::array<T, S>;
