/**
 * @file handle.hpp
 * @brief Generic handle class for resource management and lifetime tracking.
 *
 * This file defines the @ref utils::Handle template class, which provides a lightweight,
 * type-safe handle for identifying and managing resources (e.g., objects in a pool).
 * Handles store both an index and a generation, allowing validity checks and safe reuse.
 * 
 * @date 2025-08-13
 * author Terry Toupi
 */

#ifndef __HANDLE_HPP__
#define __HANDLE_HPP__

#include <cstdint>

/**
 * @brief Generic handle for resource identification and validation.
 *
 * The Handle class provides a unique identifier composed of an index and a generation.
 * Handles can be checked for validity, compared, packed/unpacked to/from an integer,
 * and used with resource pools.
 *
 * @tparam T The type of resource the handle refers to.
 */
template<typename T>
class Handle
{
public:
	/**
	 * @brief Default constructor. Initializes to an invalid handle.
	 */
	Handle() : m_Index(0), m_Generation(0) {}

	/**
	 * @brief Checks whether the handle is valid.
	 * @return true if valid, false otherwise.
	 */
	bool IsValid() const { return m_Generation != 0; }

	/**
	 * @brief Equality operator.
	 * @param other Handle to compare.
	 * @return true if both index and generation match.
	 */
	bool operator==(const Handle<T>& other) const { return other.m_Index == m_Index && other.m_Generation == m_Generation; }

	/**
	 * @brief Inequality operator.
	 * @param other Handle to compare.
	 * @return true if either index or generation differ.
	 */
	bool operator!=(const Handle<T>& other) const { return other.m_Index != m_Index || other.m_Generation != m_Generation; }

	/**
	 * @brief Returns a sentinel (invalid) handle.
	 * @return A handle with index 0 and max generation (0xffff).
	 */
	static Handle sentinal() { return Handle(0, 0xffff); }

	/**
	 * @brief Computes a hash key from the index and generation.
	 * @return A 32-bit integer uniquely representing the handle.
	 */
	uint32_t hashKey() const { return (((uint32_t)m_Index) << 16) + (uint32_t)m_Generation; }

	/**
	 * @brief Gets the index part of the handle.
	 * @return Index value (16-bit).
	 */
	uint16_t index()  const { return m_Index; }

	/**
	 * @brief Packs the handle into a 32-bit integer.
	 * @return Packed value.
	 */
	uint32_t pack() const { return (static_cast<uint32_t>(m_Index) << 16) | m_Generation; }

	/**
	 * @brief Unpacks a 32-bit integer into the handle.
	 * @param packedValue Value to unpack.
	 */
	Handle<T> unpack(uint32_t packedValue)
	{
		m_Index = static_cast<uint16_t>(packedValue >> 16);
		m_Generation = static_cast<uint16_t>(packedValue & 0xFFFF);
		return *this;
	}

private:
	/**
	 * @brief Constructs a handle from index and generation.
	 * @param index Index value.
	 * @param generation Generation value.
	 */
	Handle(uint32_t index, uint32_t generation) : m_Index(index), m_Generation(generation) {}

private:
	uint16_t m_Index;      /**< Resource index */
	uint16_t m_Generation; /**< Generation for validity tracking */

	template<typename U, typename V> friend class Pool;
};

#endif // !__HANDLE_HPP__
