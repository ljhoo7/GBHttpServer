#pragma once

#define BOOST_THREAD_VERSION 5

#include <string>
#include <vector>
#include <atomic>
#include <limits>

namespace GenericBoson
{
	std::string GetWSALastErrorString();
	std::string GetWSALastErrorString(int lastError);

	template<typename T>
	class LockFreeCircularQueue
	{
	public:
		LockFreeCircularQueue(int size) : m_size(size)
		{
			static_assert(0 < size);
			static_assert(size <= std::numeric_limits<uint32_t>::max());
		}

		void Push(const T& newElement)
		{
		}

		T Pop()
		{
			//std::scoped_lock lock(m_mutex);
		}

		bool IsFull()
		{
			return true;
		}

		bool IsEmpty()
		{
			return true;
		}
	private:
		std::mutex m_mutex;

		std::vector<T> m_queue;
		std::atomic_uint32_t m_rear = 0, m_front = 1, m_size = 0;
	};
}