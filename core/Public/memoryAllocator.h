#pragma once

#include <optional>

#include <offsetAllocator.h>
#include <bumpAllocator.h>

template<typename Ttype>
struct Global
{
	OffsetAllocator::Allocation alloc;
	Ttype*						ptr;
	int							count;

	Ttype* operator->() { return ptr; }
	Ttype& operator[](std::size_t index) { return ptr[index]; }
	const Ttype& operator[](std::size_t index) const { return ptr[index]; }

	// Range support
	Ttype* begin() { return ptr; }
	Ttype* end() { return ptr + count; }

	const Ttype* begin() const { return ptr; }
	const Ttype* end()   const { return ptr + count; }
};

template<typename Ttype>
struct Frame
{
	Ttype*		ptr;
	int			count;

	Ttype* operator->() { return ptr; }
	Ttype& operator[](std::size_t index) { return ptr[index]; }
	const Ttype& operator[](std::size_t index) const { return ptr[index]; }

	// Range support
	Ttype* begin() { return ptr; }
	Ttype* end() { return ptr + count; }

	const Ttype* begin() const { return ptr; }
	const Ttype* end()   const { return ptr + count; }
};

class Allocator
{
public:
	Allocator() = default;
	Allocator(const Allocator& other) = delete;
	Allocator(Allocator&& other) = delete;

	auto GetGlobalAllocator() -> OffsetAllocator::Allocator& { return m_globalAllocator.value(); }
	auto GetFrameAllocator() -> BumpAllocator& { return m_frameAllocator.value(); }

	auto GetGlobalData() -> void* 
	{ 
		auto* copied(m_globalData);
		return copied; 
	}
	auto GetFrameData() -> void* 
	{ 
		auto* copied(m_frameData);
		return copied; 
	}

	static auto GetInstance() -> Allocator& { return s_instance; }
	static auto GetInstanceConst() -> const Allocator& { return s_instance; }

	void SetRegionsCapacity(size_t globalCapacity, size_t frameCapacity);
	static void ResetFrameRegion();

	template<typename Ttype>
	static void Remove(Global<Ttype>& ptr)
	{
		if (ptr.ptr)
			ptr.ptr->~Ttype();

#if not defined(NDEBUG)
		auto& allocator = GetInstance().GetGlobalAllocator();
		auto globalData = static_cast<char*>(GetInstance().GetGlobalData());
		std::memset(globalData + ptr.alloc.offset, 0xDD, sizeof(Ttype));
#endif
		GetInstance().GetGlobalAllocator().free(ptr.alloc);
		ptr.ptr = nullptr;
	}

	template<typename Ttype, typename... Args>
	static Global<Ttype> GlobalAlloc(int capacity = 1, Args&&... args)
	{
		OffsetAllocator::Allocation alloc = GetInstance().GetGlobalAllocator().allocate(sizeof(Ttype) * capacity);
		Ttype* ptr = (Ttype*)((char*)GetInstance().GetGlobalData() + alloc.offset);
		
		auto* copied(ptr);
		for (int i = 0; i < capacity; ++i)
			new (copied + i) Ttype(std::forward<Args>(args)...);

		return { alloc, ptr, capacity };
	}

	template<typename Ttype, typename... Args>
	static Frame<Ttype> FrameAlloc(int capacity = 1, Args&&... args)
	{
		size_t alloc = GetInstance().GetFrameAllocator().allocate(sizeof(Ttype) * capacity, alignof(Ttype));
		Ttype* ptr = (Ttype*)((char*)GetInstance().GetFrameData() + alloc);
	
		auto* copied(ptr);
		for (int i = 0; i < capacity; ++i)
			new (copied + i) Ttype(std::forward<Args>(args)...);

		return { ptr, capacity };
	}

private:
	void* m_globalData{};
	std::optional<OffsetAllocator::Allocator> m_globalAllocator;

	void* m_frameData{};
	std::optional<BumpAllocator> m_frameAllocator;

private:
	static Allocator s_instance;
};
