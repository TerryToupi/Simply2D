#pragma once

#include "EABase/eabase.h"

namespace eastl
{
	class allocator;
	inline bool operator==(allocator const&, allocator const&) { return true; }
	inline bool operator!=(allocator const&, allocator const&) { return false; }
}

