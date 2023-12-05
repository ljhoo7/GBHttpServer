#pragma once

#include <atomic>

namespace GenericBoson
{
	template<typename SUBJECT>
	class Singleton
	{
	public:
		SUBJECT* GetInstance()
		{
			SUBJECT* pInstance = m_pInstance.load(std::memory_order_relaxed);
			std::atomic_thread_fence(std::memory_order_acquire);
			if (pInstance == nullptr)
			{
				std::lock_guard<std::mutex> lock(m_Lock);
				pInstance = m_pInstance.load(std::memory_order_relaxed);
				if (pInstance == nullptr)
				{
					pInstance = new SUBJECT;
					std::atomic_thread_fence(std::memory_order_release);
					m_pInstance.store(pInstance, std::memory_order_relaxed);
				}
			}

			return pInstance;
		}
	protected:
		Singleton() = default;
		Singleton(const Singleton&) = delete;
		Singleton(const Singleton&&) = delete;
		Singleton& operator=(const Singleton&) = delete;
	public:
	private:
		std::mutex m_Lock;
		std::atomic<SUBJECT*> m_pInstance;
	};
}