#pragma once

#include "String.h"

#include "Memory/memory.h"
#include "EASTL/hash_map.h"

template<typename K, typename V> 
using THashMap = eastl::hash_map<K, V, std::hash<K>>;

template<typename V>
using TStringHashMap = eastl::hash_map<TString, V, eastl::string_hash<TString>, eastl::equal_to<TString>>;

template<typename K, typename V> 
using TPair = eastl::pair<K, V>;