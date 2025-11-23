#pragma once

#include "Memory/memory.h"
#include "EASTL/hash_map.h"

template<typename K, typename V> 
using THashMap = eastl::hash_map<K, V, std::hash<K>>;

template<typename K, typename V> 
using TPair = eastl::pair<K, V>;