#include <pch.h>

#include <memoryAllocator.h>

Allocator Allocator::s_instance;

void Allocator::SetRegionsCapacity(const AllocatorSpecifications& specs)
{
	if (m_frameAllocator || m_globalAllocator)
		return;

	m_globalAllocator.emplace(specs.globalCapacity);
	m_globalData = operator new(specs.globalCapacity);

	m_sceneAllocator.emplace((OffsetAllocator::uint32)specs.sceneCapacity, 128 * 1024);
	m_sceneData = operator new(specs.sceneCapacity);

	m_frameAllocator.emplace(specs.frameCapacity);
	m_frameData = operator new(specs.frameCapacity);
}

void Allocator::ResetFrameRegion()
{
	GetInstance().GetFrameAllocator().reset();
}
