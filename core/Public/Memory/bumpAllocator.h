#pragma once

#pragma once
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <algorithm>

class BumpAllocator
{
public:
    BumpAllocator(size_t capacity);

    void reset() noexcept;
    /**
     * @brief Allocates a block of memory and returns its offset (from base).
     *
     * @param size Size of the allocation in bytes.
     * @param alignment Desired memory alignment.
     * @return Offset from the beginning of the region.
     */
    size_t allocate(size_t size, size_t alignment = alignof(std::max_align_t));

    size_t used() const noexcept { return m_offset; }
    size_t capacity() const noexcept { return m_capacity; }

private:
    static size_t alignUp(size_t offset, size_t alignment)
    {
        return (offset + alignment - 1) & ~(alignment - 1);
    }

    size_t m_capacity = 0;
    size_t m_offset = 0;
};
