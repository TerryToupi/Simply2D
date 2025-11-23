#pragma once

#include "Memory/memory.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/unique_ptr.h"

template<typename T>
using Ref = eastl::shared_ptr<T>;

template<typename T, typename... Args>
inline Ref<T> MakeRef(Args&&... args)
{
	return eastl::make_shared<T>(eastl::forward<Args>(args)...);
}

template<typename T>
using Scope = eastl::unique_ptr<T>;

template<typename T, typename... Args>
inline Scope<T> MakeScope(Args&&... args)
{
	return eastl::make_unique<T>(eastl::forward<Args>(args)...);
}