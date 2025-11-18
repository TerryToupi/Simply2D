#pragma once

#include "Memory/allocator.h"
#include "EASTL/hash_map.h"

template<typename K, typename V> 
using THashMap = eastl::hash_map<K, V, eastl::hash<K>, eastl::equal_to<K>, MM::RPMallocAllocator>;

template<typename K, typename V> 
using TPair = eastl::pair<K, V>;