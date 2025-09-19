/**
 * @file pool.hpp
 * @brief Generic object pool with handle-based access and lifetime management.
 *
 * This file defines the @ref utils::Pool class template, providing efficient
 * storage, allocation, and deallocation for objects using lightweight handles.
 * The pool tracks object generations to prevent stale handle access and enables
 * fast insertion, removal, and retrieval operations.
 *
 * @date 2025-08-13
 * author Terry Toupi
 */

#ifndef __POOL_HPP__
#define __POOL_HPP__

#include <vector>
#include <string>
#include <handle.h>

/**
 * @brief Generic pool for managing objects and tracking their lifetimes using handles.
 *
 * The Pool class enables fast allocation and deallocation of objects of type U.
 * Each object is identified by a handle of type V, which includes an index and generation for safe access.
 * Handles become invalid when their generation is incremented (i.e., after object removal).
 *
 * @tparam U Type of objects stored in the pool.
 * @tparam V Type used for handle generation.
 */
template<typename U, typename V>
class Pool {
public:
	/**
	 * @brief Constructs a pool with reserved capacity.
	 * @param reserveSize Initial number of objects to reserve space for.
	 * @param debugName Name for debugging purposes.
	 */
	Pool(uint32_t reserveSize, std::string debugName)
		: m_DebugName(std::move(debugName)), m_Size(reserveSize)
	{
		m_Data.resize(m_Size);
		m_Generation.resize(m_Size, 1);

		// Initialize free list in reverse order for efficient allocation.
		for (uint32_t i = 0; i < m_Size; i++) {
			m_FreeList.emplace_back((m_Size - 1) - i);
		}
	}

	/**
	 * @brief Inserts an object into the pool and returns a handle.
	 *
	 * If the pool is full, it will automatically resize.
	 *
	 * @param data The object to insert.
	 * @return A handle identifying the inserted object.
	 */
	Handle<V> Insert(const U& data)
	{
		if (m_FreeList.empty()) {
			Resize(m_Size * 2);
		}

		uint32_t index = m_FreeList.back();
		m_FreeList.pop_back();

		m_Data[index] = std::move(data);

		return { index, m_Generation[index] };
	}

	/**
	 * @brief Retrieves a pointer to the object identified by a handle.
	 *
	 * @param handle The handle for the object.
	 * @return Pointer to the object, or nullptr if the handle is invalid or stale.
	 */
	U* Get(Handle<V> handle)
	{
		if (!handle.IsValid() || handle.m_Generation != m_Generation[handle.m_Index]) {
			return nullptr;
		}

		return &m_Data[handle.m_Index];
	}

	/**
	 * @brief Removes the object identified by a handle from the pool.
	 *
	 * After removal, the handle becomes invalid and cannot be used to access the object.
	 *
	 * @param handle The handle for the object to remove.
	 */
	void Remove(Handle<V> handle)
	{
		if (handle.m_Index >= m_Size || m_Generation[handle.m_Index] != handle.m_Generation) {
			return;
		}

		m_Generation[handle.m_Index]++;
		m_FreeList.emplace_back(handle.m_Index);
	}

private:
	/**
	 * @brief Resizes the pool to a new size, preserving all current objects.
	 * @param newSize The new pool size.
	 */
	void Resize(uint32_t newSize)
	{
		m_Data.resize(newSize);
		m_Generation.resize(newSize, 1);

		// Add new entries to the free list.
		for (uint32_t i = 0; i < (newSize - m_Size); i++) {
			m_FreeList.emplace_back((newSize - 1) - i);
		}

		m_Size = newSize;
	}

private:
	std::vector<U> m_Data;           /**< Storage for objects */
	std::vector<uint16_t> m_Generation; /**< Generation tracking for handles */
	std::vector<uint16_t> m_FreeList;   /**< List of available indices */
	std::string m_DebugName;         /**< Pool name for debugging */
	uint32_t m_Size;                 /**< Current pool size */
};

#endif // !__POOL_HPP__
