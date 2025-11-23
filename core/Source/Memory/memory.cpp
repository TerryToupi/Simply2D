#include "Memory/memory.h"

#include <cstdio>
#include <utility>

#include "rpmalloc.h"
#include "rpnew.h"

namespace MM
{
    static bool g_isMemorySystemInitialized = false;
    static rpmalloc_config_t g_rpmallocConfig;

    void Initialize()
    {
        assert(!g_isMemorySystemInitialized);

        // Init config
        memset(&g_rpmallocConfig, 0, sizeof(rpmalloc_config_t));
        //g_rpmallocConfig.page_size = 64 * 1024;
        //g_rpmallocConfig.span_size = 128 * 1024;
        //g_rpmallocConfig.span_map_count = 32;
        g_rpmallocConfig.enable_huge_pages = 1;
        g_rpmallocConfig.span_map_count = 256;
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

    void CLIReportStatistics()
    {
        printf("<============================== [MEMORY REPORT] ==============================>\n");
        rpmalloc_dump_statistics(stdout);
    }

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
}
