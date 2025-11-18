#include "Memory/memory.h"

#include "rpmalloc.h"

#include <utility>

namespace MM
{
    static bool g_isMemorySystemInitialized = false;
    static rpmalloc_config_t g_rpmallocConfig;

    //-------------------------------------------------------------------------

    void Initialize()
    {
        assert(!g_isMemorySystemInitialized);

        // Init config
        memset(&g_rpmallocConfig, 0, sizeof(rpmalloc_config_t));
        rpmalloc_initialize_config(&g_rpmallocConfig);

        g_isMemorySystemInitialized = true;
    }

    void Shutdown()
    {
        assert(g_isMemorySystemInitialized);
        g_isMemorySystemInitialized = false;

        rpmalloc_finalize();
    }

    void InitializeThreadHeap()
    {
        // Since our tasks are not bound to a specific thread and we may alloc on one and free on another. This prevents us from calling thread finalize when we shutdown a thread
        // as we can not guarantee that we have freed everything that may have been allocated from this thread.
        // This is not a problem since on application shutdown, we call rpmalloc_finalize, which will release the thread heaps
        rpmalloc_thread_initialize();
    }

    void ShutdownThreadHeap()
    {
        rpmalloc_thread_finalize(1);
    }

    size_t GetTotalRequestedMemory()
    {
        rpmalloc_global_statistics_t stats;
        rpmalloc_global_statistics(&stats);
        return stats.mapped;
    }

    size_t GetTotalAllocatedMemory()
    {
        rpmalloc_global_statistics_t stats;
        rpmalloc_global_statistics(&stats);
        return stats.mapped_peak;
    }
}

//-------------------------------------------------------------------------

void* Alloc(size_t size, size_t alignment)
{
    assert(MM::g_isMemorySystemInitialized);

	if (size == 0) return nullptr;

	void* pMemory = nullptr;

	pMemory = rpaligned_alloc(alignment, size);
    assert(MM::IsAligned(pMemory, alignment));
	return pMemory;
}

void* Realloc(void* pMemory, size_t newSize, size_t originalAlignment)
{
    assert(MM::g_isMemorySystemInitialized);

	void* pReallocatedMemory = nullptr;

	pReallocatedMemory = rprealloc(pMemory, newSize);

    assert(pReallocatedMemory != nullptr);
	return pReallocatedMemory;
}

void Free(void*& pMemory)
{
    assert(MM::g_isMemorySystemInitialized);

	rpfree((uint8_t*)pMemory);
	pMemory = nullptr;
}
