#include <pch.h>

#include <memoryAllocator.h>

Allocator Allocator::s_instance;

void Allocator::SetRegionsCapacity(size_t globalCapacity, size_t frameCapacity)
{
	if (m_frameAllocator || m_globalAllocator)
		return;

	m_globalAllocator.emplace((OffsetAllocator::uint32)globalCapacity, 128 * 1024);
	m_globalData = operator new(globalCapacity);

	m_frameAllocator.emplace(frameCapacity);
	m_frameData = operator new(frameCapacity);
}

void Allocator::ResetFrameRegion()
{
	GetInstance().GetFrameAllocator().reset();
}
