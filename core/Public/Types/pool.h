#pragma once

#include "Handle.h"
#include "Arrays.h"

template<typename U, typename V, uint16_t MinimumPoolSize = 16>
class TPool 
{
private:
    static constexpr float const GrothRate = 1.5;

    struct Slot
    {
        U        data;
        uint16_t generation = 0;
    };

public:
    TPool(uint16_t reserveSize = MinimumPoolSize)
    {
        Grow(reserveSize);
    }

    THandle<V> Insert(U&& data)
    {
        if (m_freeList.empty())
        {
            uint16_t newSize = eastl::max<uint16_t>(
                uint16_t(m_size * GrothRate),
                uint16_t(m_size + 1)
            );
            Grow(newSize);
        }

        uint16_t index = m_freeList.back();
        m_freeList.pop_back();

        m_data[index].data = std::move(data);
        return { index, m_data[index].generation };
    }

    U* Get(const THandle<V>& handle)
    {
        if (handle.m_index >= m_size) 
            return nullptr;
        if (handle.m_generation != m_data[handle.m_index].generation) 
            return nullptr;

        return &m_data[handle.m_index].data;
    }

    void Remove(const THandle<V>& handle)
    {
        if (handle.m_index >= m_size) return;
        if (m_data[handle.m_index].generation != handle.m_generation) return;

        ++m_data[handle.m_index].generation;
        m_freeList.push_back(handle.m_index);
    }

    inline U*       operator[](const THandle<V>& handle)       { return Get(handle); }
    inline U const* operator[](const THandle<V>& handle) const { return Get(handle); }

private:
    void Grow(uint16_t newSize)
    {
        uint16_t oldSize = m_size;
        m_size = newSize;

        m_data.resize(m_size);
        for (uint16_t i = newSize; i-- > oldSize;)
            m_freeList.push_back(i);
    }

private:
    uint16_t                                 m_size = 0;
    TInlineVector<Slot, MinimumPoolSize>     m_data;
    TInlineVector<uint16_t, MinimumPoolSize> m_freeList;
};
