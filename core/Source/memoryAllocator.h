#pragma once

#include <optional>

#include <Source/offsetAllocator.h>

using namespace OffsetAllocator;

template<typename Ttype>
struct MemoryAllocation
{
	Allocation allocation;
	Ttype* ptr{};
};

class MemoryAllocator
{
public:
	MemoryAllocator() = default;
	MemoryAllocator(const MemoryAllocator& other) = delete;
	MemoryAllocator(MemoryAllocator&& other) = delete;

	void SetCapacity(size_t globalCapacity, size_t frameCapacity);

	auto GetGlobalAllocator() -> Allocator& { return m_globalAllocator.value(); }
	auto GetFrameAllocator() -> Allocator& { return m_frameAllocator.value(); }

	auto GetGlobalData() -> void* { return m_globalData; }
	auto GetFrameData() -> void* { return m_frameData; }

	void ResetFrameData() { m_frameAllocator.reset(); }

	static auto GetInstance() -> MemoryAllocator& { return s_instance; }
	static auto GetInstanceConst() -> const MemoryAllocator& { return s_instance; }
	
	template<typename Ttype>
	static MemoryAllocation<Ttype> Global()
	{
		Allocation alloc = GetInstance().GetGlobalAllocator().allocate(sizeof(Ttype));
		Ttype* ptr = (Ttype*)((char*)GetInstance().GetGlobalData() + alloc.offset);
		return {alloc, ptr};
	}

	template<typename Ttype>
	static MemoryAllocation<Ttype> Frame()
	{
		Allocation alloc = GetInstance().GetFrameAllocator().allocate(sizeof(Ttype));
		Ttype* ptr = (Ttype*)((char*)GetInstance().GetFrameData() + alloc.offset);
		return {alloc, ptr};
	}

private:
	void* m_globalData{};
	std::optional<Allocator> m_globalAllocator;

	void* m_frameData{};
	std::optional<Allocator> m_frameAllocator;

private:
	static MemoryAllocator s_instance;
};
