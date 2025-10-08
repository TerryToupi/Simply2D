#include <pch.h>

#include <bumpAllocator.h>

BumpAllocator::BumpAllocator(size_t capacity)
{
    m_capacity = capacity;
    m_offset = 0;
}

void BumpAllocator::reset() noexcept
{
    m_offset = 0;
}

size_t BumpAllocator::allocate(size_t size, size_t alignment)
{
    size_t alignedOffset = alignUp(m_offset, alignment);
    size_t newOffset = alignedOffset + size;

    assert(newOffset <= m_capacity && "Frame allocator overflow!");

    m_offset = newOffset;
    return alignedOffset;
}

