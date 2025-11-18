#pragma once

#include "Memory/allocator.h"
#include "EASTL/string.h"
#include "EASTL/fixed_string.h"

using TString = eastl::basic_string<char, MM::RPMallocAllocator>;

template<eastl_size_t S>
using TInlineString = eastl::fixed_string<char, 256, true, MM::RPMallocAllocator>;
