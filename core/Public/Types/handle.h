#pragma once
#include <cstdint>

template<typename T>
class THandle
{
public:
	THandle() : m_index(0), m_generation(0) {}
	THandle(uint32_t packed) : m_index(static_cast<uint16_t>(packed >> 16)), m_generation(static_cast<uint16_t>(packed & 0xFFFF)) {}

	bool IsValid() const { return m_generation != 0; }

	bool operator==(const THandle<T>& other) const { return other.m_index == m_index && other.m_generation == m_generation; }
	bool operator!=(const THandle<T>& other) const { return other.m_index != m_index || other.m_generation != m_generation; }

	static THandle Sentinal() { return THandle(0, 0xffff); }

	uint32_t Hash() const { return (((uint32_t)m_index) << 16) + (uint32_t)m_generation; }
	uint16_t Index()  const { return m_index; }
	uint16_t Generation() const { return m_generation; }

	uint32_t Pack() const { return (static_cast<uint32_t>(m_index) << 16) | m_generation; }

private:
	THandle(uint32_t index, uint32_t generation) : m_index(index), m_generation(generation) {}

private:
	uint16_t m_index; 
	uint16_t m_generation;

	template<typename U, typename V, uint16_t MinimumPoolSize>
	friend class TPool;
};
