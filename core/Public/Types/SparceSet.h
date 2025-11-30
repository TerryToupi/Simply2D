#pragma once

#include "Arrays.h"
#include "Utils.h"

template<typename T>
class TSparceSet
{
public:
	static constexpr size_t RESERVE_SIZE = 1024;

	TSparceSet()
	{
		m_dense.reserve(RESERVE_SIZE);
		m_indexToId.reserve(RESERVE_SIZE);
	}

	T* Add(uint32_t id, T&& data)
	{
		size_t index = GetDenseIndex(id);
		if (index != INVALID)
		{
			m_dense[index] = TMove(data);
			m_indexToId[index] = id;
			return &m_dense[index];
		}

		SetDenseIndex(id, m_dense.size());
		m_dense.push_back(TMove(data));
		m_indexToId.push_back(id);
		return &m_dense.back();
	}

	T* Get(uint32_t id)
	{
		size_t index = GetDenseIndex(id);
		if (index != INVALID)
			return &m_dense[index];

		return nullptr;
	}

	void Remove(uint32_t id)
	{
		size_t index = GetDenseIndex(id);
		if (index == INVALID)
			return;

		size_t lastIndex = m_dense.size() - 1;
		uint32_t lastId	 = m_indexToId[lastIndex];

		if (index != lastIndex)
		{
			TSwap(m_dense[index], m_dense[lastIndex]);
			TSwap(m_indexToId[index], m_indexToId[lastIndex]);
			SetDenseIndex(lastId, index);
		}

		SetDenseIndex(id, INVALID);

		m_dense.pop_back();
		m_indexToId.pop_back();
	}

	bool HasId(uint32_t id)
	{
		return GetDenseIndex(id) != INVALID;
	}

	void Clear()
	{
		m_dense.clear();
		m_sparce.clear();
		m_indexToId.clear();
	}

	bool Empty()
	{
		return m_dense.empty();
	}

	size_t NumberOfIds()
	{
		return m_indexToId.size();
	}

private:
	inline void SetDenseIndex(uint32_t id, size_t index)
	{
		size_t pageIndex = id / PAGE_MAX_SIZE;
		size_t slotIndex = id % PAGE_MAX_SIZE;

		if (pageIndex >= m_sparce.size())
		{
			m_sparce.resize(pageIndex + 1);
			m_sparce[pageIndex].fill(INVALID);
		}

		Page& page = m_sparce[pageIndex];
		page[slotIndex] = index;
	}

	inline size_t GetDenseIndex(uint32_t id)
	{
		size_t pageIndex = id / PAGE_MAX_SIZE;
		size_t slotIndex = id % PAGE_MAX_SIZE;

		if (pageIndex < m_sparce.size())
		{
			Page& page = m_sparce[pageIndex];
			return page[slotIndex];
		}

		return INVALID;
	}

private:
	static constexpr size_t PAGE_MAX_SIZE = 2048;
	static constexpr size_t INVALID = std::numeric_limits<size_t>::max();
	using Page = TArray<size_t, PAGE_MAX_SIZE>;

	TVector<T>			m_dense;
	TVector<Page>		m_sparce;
	TVector<uint32_t>	m_indexToId;
};
