#pragma once

#include "Memory/memory.h"
#include "EASTL/string.h"
#include "EASTL/fixed_string.h"

using TString = eastl::basic_string<char>;

template<eastl_size_t S>
using TInlineString = eastl::fixed_string<char, 256, true>;
