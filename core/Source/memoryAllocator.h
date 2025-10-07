#pragma once

#include <optional>

#include <Source/offsetAllocator.h>

template<typename Ttype>
struct Global
{
	OffsetAllocator::Allocation alloc;
	Ttype* ptr;

	// Overload the -> operator
	Ttype* operator->() { return ptr; }
};

template<typename Ttype>
struct Frame
{
	Ttype* ptr;

	// Overload the -> operator
	Ttype* operator->() { return ptr; }
};

class Allocator
{
public:
	Allocator() = default;
	Allocator(const Allocator& other) = delete;
	Allocator(Allocator&& other) = delete;

	auto GetGlobalAllocator() -> OffsetAllocator::Allocator& { return m_globalAllocator.value(); }
	auto GetFrameAllocator() -> OffsetAllocator::Allocator& { return m_frameAllocator.value(); }

	auto GetGlobalData() -> void* { return m_globalData; }
	auto GetFrameData() -> void* { return m_frameData; }

	static auto GetInstance() -> Allocator& { return s_instance; }
	static auto GetInstanceConst() -> const Allocator& { return s_instance; }

	void SetRegionsCapacity(size_t globalCapacity, size_t frameCapacity);
	static void ResetFrameRegion();

	template<typename Ttype>
	static void Remove(Global<Ttype>& ptr)
	{
		if (ptr.ptr)
			ptr.ptr->~Ttype();

		GetInstance().GetGlobalAllocator().free(ptr.alloc);
		ptr.ptr = nullptr;
	}

	template<typename Ttype, typename... Args>
	static Global<Ttype> Global(Args&&... args)
	{
		OffsetAllocator::Allocation alloc = GetInstance().GetGlobalAllocator().allocate(sizeof(Ttype));
		Ttype* ptr = (Ttype*)((char*)GetInstance().GetGlobalData() + alloc.offset);
		return { alloc, new (ptr) Ttype(std::forward<Args>(args)...) };
	}

	template<typename Ttype, typename... Args>
	static Frame<Ttype> Frame(Args&&... args)
	{
		OffsetAllocator::Allocation alloc = GetInstance().GetFrameAllocator().allocate(sizeof(Ttype));
		Ttype* ptr = (Ttype*)((char*)GetInstance().GetFrameData() + alloc.offset);
		return { new (ptr) Ttype(std::forward<Args>(args)...) };
	}

private:
	void* m_globalData{};
	std::optional<OffsetAllocator::Allocator> m_globalAllocator;

	void* m_frameData{};
	std::optional<OffsetAllocator::Allocator> m_frameAllocator;

private:
	static Allocator s_instance;
};
