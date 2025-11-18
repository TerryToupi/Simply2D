#pragma once

#include "Arrays.h"
#include "HashMap.h"

#include <cassert>

template<typename ID, typename Item>
class TIDVector
{
public:
	Item& operator[](int32_t idx) { return m_vector[idx]; }
	int32_t size() const { return (int32_t)m_vector.size(); }
	bool empty() const { return m_vector.empty(); }

	typename TVector<Item>::iterator begin() { return m_vector.begin(); }
	typename TVector<Item>::iterator end() { return m_vector.end(); }

	typename TVector<Item>::const_iterator begin() const { return m_vector.begin(); }
	typename TVector<Item>::const_iterator end()   const { return m_vector.end(); }

	bool HasItemWithID(ID const& id) const
	{
		return m_indexMap.find(id) != m_indexMap.end();
	}

	Item* Add(Item const& item)
	{
		ID const id = GetItemID(std::is_pointer<Item>(), item);
		assert(m_indexMap.find(id) == m_indexMap.end());
		int32_t const itemIdx = (int32_t)m_vector.size();
		m_vector.emplace_back(item);
		m_indexMap.insert(TPair<ID, int32_t>(id, itemIdx));
		return &m_vector[itemIdx];
	}

	Item* Add(Item const&& item)
	{
		ID const id = GetItemID(std::is_pointer<Item>(), item);
		assert(m_indexMap.find(id) == m_indexMap.end());
		int32_t const itemIdx = (int32_t)m_vector.size();
		m_vector.emplace_back(eastl::forward<Item const>(item));
		m_indexMap.insert(TPair<ID, int32_t>(id, itemIdx));
		return &m_vector[itemIdx];
	}

	template<class... Args>
	Item* Emplace(ID const& id, Args&&... args)
	{
		assert(m_indexMap.find(id) == m_indexMap.end());
		int32_t const itemIdx = (int32_t)m_vector.size();
		m_vector.emplace_back(eastl::forward<Args>(args)...);
		assert(GetLastElementID(std::is_pointer<Item>()) == id);
		m_indexMap.insert(TPair<ID, int32_t>(id, itemIdx));
		return &m_vector[itemIdx];
	}

	void Remove(ID const& id)
	{
		auto foundIter = m_indexMap.find(id);
		assert(foundIter != m_indexMap.end());

		if (m_vector.size() > 1)
		{
			ID lastItemID = GetLastElementID(std::is_pointer<Item>());
			auto foundLastItemIter = m_indexMap.find(lastItemID);
			foundLastItemIter->second = foundIter->second;
		}

		m_vector.erase_unsorted(m_vector.begin() + foundIter->second);
		m_indexMap.erase(foundIter);
	}

	// Not sure if the item is really in the there
	// Could return a nullptr
	Item* FindItem(ID const& id)
	{
		Item* pFound = nullptr;

		auto foundIter = m_indexMap.find(id);
		if (foundIter != m_indexMap.end())
			pFound = &m_vector[foundIter->second];

		return pFound;
	}

	template<class... Args>
	Item* FindOrAdd(ID const& id, Args&&... args)
	{
		auto pFoundItem = FindItem(id);
		if (pFoundItem == nullptr)
		{
			pFoundItem = Emplace(id, eastl::forward<Args>(args)...);
		}

		return pFoundItem;
	}

	// Has the id to exists otherwise 
	// loud crash
	Item* Get(ID const& id)
	{
		auto pFoundItem = FindItem(id);
		assert(pFoundItem != nullptr);
		return pFoundItem;
	}

protected:
	inline ID GetItemID(std::true_type, Item const& pItem) const
	{
		return pItem->GetID();
	}

	inline ID GetItemID(std::false_type, Item const& item) const
	{
		return item.GetID();
	}

	inline ID GetLastElementID(std::true_type) const
	{
		assert(!m_vector.empty());
		return m_vector.back()->GetID();
	}

	inline ID GetLastElementID(std::false_type) const
	{
		assert(!m_vector.empty());
		return m_vector.back().GetID();
	}

private:
	TVector<Item>			m_vector;
	THashMap<ID, int32_t>	m_indexMap;
};