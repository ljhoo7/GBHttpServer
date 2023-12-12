#pragma once

#include <atomic>
#include <mutex>

namespace GenericBoson
{
	template<typename SUBJECT>
	class Singleton
	{
	public:
		static SUBJECT* GetInstance()
		{
			static std::mutex g_Lock;
			static std::atomic<SUBJECT*> g_pInstance;

			SUBJECT* pInstance = g_pInstance.load(std::memory_order_relaxed);
			std::atomic_thread_fence(std::memory_order_acquire);
			if (pInstance == nullptr)
			{
				std::lock_guard<std::mutex> lock(g_Lock);
				pInstance = g_pInstance.load(std::memory_order_relaxed);
				if (pInstance == nullptr)
				{
					pInstance = new SUBJECT;
					std::atomic_thread_fence(std::memory_order_release);
					g_pInstance.store(pInstance, std::memory_order_relaxed);
				}
			}

			return pInstance;
		}
	protected:
		Singleton() = default;
		Singleton(const Singleton&) = delete;
		Singleton(const Singleton&&) = delete;
		Singleton& operator=(const Singleton&) = delete;
	};
}