#include <pch.h>

#include <Source/memoryAllocator.h>

MemoryAllocator MemoryAllocator::s_instance;

void MemoryAllocator::SetCapacity(size_t globalCapacity, size_t frameCapacity)
{
	if (m_frameAllocator.has_value() || m_globalAllocator.has_value())
		return;

	m_globalAllocator.emplace(Allocator(globalCapacity));
	m_globalData = operator new(globalCapacity);

	m_frameAllocator.emplace(Allocator(frameCapacity));
	m_frameData = operator new(frameCapacity);
}
