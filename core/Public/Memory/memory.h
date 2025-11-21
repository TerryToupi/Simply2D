#pragma once

#include <type_traits>
#include <cassert>
#include <cstring>

#define DEFAULT_ALIGNMENT 8

namespace MM
{
    void Initialize();
    void Shutdown();

    void InitializeThreadHeap();
    void ShutdownThreadHeap();

    inline void MemsetZero(void* ptr, size_t size)
    {
        memset(ptr, 0, size);
    }

    template<typename T>
    inline void MemsetZero(T* ptr)
    {
        memset(ptr, 0, sizeof(T));
    }

    inline bool IsAligned(void const* p, size_t n)
    {
        return (reinterpret_cast<uintptr_t>(p) % n) == 0;
    }

    template <typename T>
    inline bool IsAligned(T const* p)
    {
        return (reinterpret_cast<uintptr_t>(p) % alignof(T)) == 0;
    }

    inline size_t CalculatePaddingForAlignment(uintptr_t addressOffset, size_t requiredAlignment)
    {
        return (requiredAlignment - (addressOffset % requiredAlignment)) % requiredAlignment;
    }

    inline size_t CalculatePaddingForAlignment(void* address, size_t requiredAlignment)
    {
        return CalculatePaddingForAlignment(reinterpret_cast<uintptr_t>(address), requiredAlignment);
    }

    size_t GetTotalRequestedMemory();
    size_t GetTotalAllocatedMemory();
    void   CLIReportStatistics();
}

//-------------------------------------------------------------------------
// Global Memory Management Functions
//-------------------------------------------------------------------------

[[nodiscard]] void* Alloc(size_t size, size_t alignment = DEFAULT_ALIGNMENT);
[[nodiscard]] void* Realloc(void* pMemory, size_t newSize, size_t originalAlignment = DEFAULT_ALIGNMENT);
void Free(void*& pMemory);

template<typename T, typename... ConstructorParams>
[[nodiscard]] inline T* New(ConstructorParams&&... params)
{
	void* pMemory = Alloc(sizeof(T), alignof(T));
    assert(pMemory != nullptr);
	return new(pMemory) T(std::forward<ConstructorParams>(params)...);
}

template<typename T>
inline void Delete(T*& pType)
{
	if (pType != nullptr)
	{
		pType->~T();
		Free((void*&)pType);
	}
}

