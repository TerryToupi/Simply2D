#pragma once

#include "Memory/allocator.h"
#include "EASTL/deque.h"

template<typename T>
using TDeque = eastl::deque<T, MM::RPMallocAllocator>;
