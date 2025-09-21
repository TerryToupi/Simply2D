#ifndef __POOL_HPP__
#define __POOL_HPP__

#include <vector>
#include <mutex>
#include <string>
#include <handle.h>

/**
 * @brief Thread-safe pool for managing objects with handle-based access.
 */
template<typename U, typename V>
class Pool {
public:
    explicit Pool(uint32_t reserveSize, std::string debugName = {})
        : m_DebugName(std::move(debugName))
    {
        grow(reserveSize);
    }

    Handle<V> generateHandle()
    {
        std::lock_guard<std::mutex> lock(m_Mutex);

        uint16_t index = m_FreeList.back();
        return { index, m_Generation[index] }; 
    }

    Handle<V> insert(const U& data) 
    {
        std::lock_guard<std::mutex> lock(m_Mutex);

        uint16_t index = m_FreeList.back();
        m_FreeList.pop_back();

        m_Data[index] = data; // copy/move assignment

        return { index, m_Generation[index] }; 
    }

    void insert(Handle<V> handle, const U& data)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);

        if (handle.index() == m_FreeList.back())
        {
            m_FreeList.pop_back();
            m_Data[handle.index()] = data;
        }
    }

    U* get(const Handle<V>& handle) 
    {
        std::lock_guard<std::mutex> lock(m_Mutex);

        if (!handle.IsValid() || handle.m_Index >= m_Size) return nullptr;
        if (handle.m_Generation != m_Generation[handle.m_Index]) return nullptr;

        return &m_Data[handle.m_Index];
    }

    void remove(const Handle<V>& handle) 
    {
        std::lock_guard<std::mutex> lock(m_Mutex);

        if (!handle.IsValid() || handle.m_Index >= m_Size) return;
        if (m_Generation[handle.m_Index] != handle.m_Generation) return;

        m_Generation[handle.m_Index]++;
        m_FreeList.push_back(handle.m_Index);
    }

private:
    void grow(uint16_t newSize)
    {
        uint16_t oldSize = m_Size;
        m_Size = newSize;

        m_Data.resize(m_Size);
        m_Generation.resize(m_Size, 1);

        for (uint16_t i = oldSize; i < m_Size; ++i)
        {
            m_FreeList.push_back((m_Size - 1) - (i - oldSize));
        }
    }

private:
    std::vector<U> m_Data;
    std::vector<uint16_t> m_Generation;
    std::vector<uint16_t> m_FreeList;

    std::mutex m_Mutex;
    std::string m_DebugName;
    uint16_t m_Size = 0;
};

#endif // __POOL_HPP__
