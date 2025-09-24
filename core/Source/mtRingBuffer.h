#pragma once

#include <algorithm>    // std::max
#include <atomic>    // to use std::atomic<uint64_t>
#include <thread>    // to use std::thread
#include <condition_variable>    // to use std::condition_variable
#include <sstream>

namespace Simply2D
{
	// Fixed size very simple thread safe ring buffer
	template <typename T, size_t capacity>
	class MTRingQueue 
	{
	public:
		// Push an item to the end if there is free space
		//	Returns true if succesful
		//	Returns false if there is not enough space
		inline bool push_back(const T& item)
		{
			std::lock_guard<std::mutex> lock(m_lock);

			bool result = false;
			size_t next = (m_head + 1) % capacity;

			if (next != m_tail)
			{
				m_data[m_head] = item;
				m_head = next;
				result = true;
			}

			return result;
		}

		// Get an item if there are any
		//	Returns true if succesful
		//	Returns false if there are no items
		inline bool pop_front(T& item)
		{
			std::lock_guard<std::mutex> lock(m_lock);

			bool result = false;

			if (m_tail != m_head)
			{
				item = m_data[m_tail];
				m_tail = (m_tail + 1) % capacity;
				result = true;
			}
			return result;
		}

	private:
		T m_data[capacity];
		size_t m_head = 0;
		size_t m_tail = 0;
		std::mutex m_lock; // this just works better than a spinlock here (on windows)
	};
}