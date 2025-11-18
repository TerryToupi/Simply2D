#pragma once

#include "Memory/allocator.h"
#include "EASTL/vector.h"
#include "EASTL/fixed_vector.h"
#include "EASTL/array.h"

template<typename T> 
using TVector = eastl::vector<T, MM::RPMallocAllocator>;

template<typename T, eastl_size_t S, typename OverflowAllocator = typename eastl::conditional<true, MM::RPMallocAllocator, EASTLDummyAllocatorType>::type>
using TInlineVector = eastl::fixed_vector<T, S, true, OverflowAllocator>;

template<typename T, eastl_size_t S> 
using TArray = eastl::array<T, S>;