#ifndef __POOL_HPP__
#define __POOL_HPP__

#include <vector>
#include <mutex>
#include <string>
#include <handle.h>

template<typename U, typename V>
class Pool {
public:
    explicit Pool(uint32_t reserveSize, std::string debugName = {})
        : m_debugName(std::move(debugName))
    {
        grow(reserveSize);
    }

    Handle<V> insert(const U& data) 
    {
        uint16_t index = m_freeList.back();
        m_freeList.pop_back();

        m_data[index] = data; // copy/move assignment

        return { index, m_generation[index] }; 
    }

    U* get(const Handle<V>& handle) 
    {
        if (!handle.IsValid() || handle.m_Index >= m_size) return nullptr;
        if (handle.m_Generation != m_generation[handle.m_Index]) return nullptr;

        return &m_data[handle.m_Index];
    }

    void remove(const Handle<V>& handle) 
    {
        if (!handle.IsValid() || handle.m_Index >= m_size) return;
        if (m_generation[handle.m_Index] != handle.m_Generation) return;

        m_generation[handle.m_Index]++;
        m_freeList.push_back(handle.m_Index);
    }

private:
    void grow(uint16_t newSize)
    {
        uint16_t oldSize = m_size;
        m_size = newSize;

        m_data.resize(m_size);
        m_generation.resize(m_size, 1);

        for (uint16_t i = oldSize; i < m_size; ++i)
        {
            m_freeList.push_back((m_size - 1) - (i - oldSize));
        }
    }

private:
    std::vector<U> m_data;
    std::vector<uint16_t> m_generation;
    std::vector<uint16_t> m_freeList;

    std::string m_debugName;
    uint16_t m_size = 0;
};

#endif // __POOL_HPP__
