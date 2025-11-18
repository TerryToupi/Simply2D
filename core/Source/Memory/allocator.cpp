#include "Memory/allocator.h"
#include "Memory/memory.h"

namespace MM
{
	void* RPMallocAllocator::allocate(size_t n, int flags)
	{
		(void)flags; 
		return Alloc(n);
	}

	void* RPMallocAllocator::allocate(size_t n, size_t alignment, size_t offset, int flags)
	{
		(void)offset; 
		(void)flags;
		return Alloc(n, alignment);
	}

	void RPMallocAllocator::deallocate(void* p, size_t n)
	{
		(void)n; 
		Free(p);
	}
}
