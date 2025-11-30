#pragma once

#include "Memory/memory.h"
#include "EASTL/string.h"
#include "EASTL/fixed_string.h"

using TString = eastl::string;

template<eastl_size_t S>
using TInlineString = eastl::fixed_string<char, 256, true>;

