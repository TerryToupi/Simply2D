#pragma once

namespace MM 
{
    class RPMallocAllocator
    {
    public:
        RPMallocAllocator(const char* = nullptr) {}
        RPMallocAllocator(const RPMallocAllocator&) {}
        RPMallocAllocator(const RPMallocAllocator&, const char*) {}
        RPMallocAllocator& operator=(const RPMallocAllocator&) { return *this; }

        void* allocate(size_t n, int flags = 0);
        void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0);
        void deallocate(void* p, size_t n);

        const char* get_name() const { return "RPMallocAllocator"; }
        void set_name(const char*) {}

        friend bool operator==(const RPMallocAllocator&, const RPMallocAllocator&) { return true; }
        friend bool operator!=(const RPMallocAllocator&, const RPMallocAllocator&) { return false; }
    };
}
