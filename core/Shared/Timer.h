#pragma once

#include <thread>
#include <chrono>

namespace GenericBoson
{
	class ITimer
	{
		virtual void OnTime() = 0;
	};

	class TimerManager
	{
	public:
		TimerManager()
			: m_thread(&TimerManager::OnStart, this)
		{
		}

		~TimerManager()
		{
			if (m_thread.joinable())
			{
				m_thread.join();
			}
		}

	private:
		void OnStart()
		{
			const auto startTime = std::chrono::steady_clock::now();

			while (m_keepGoing)
			{
			}
		}
		
	public:
		std::atomic_bool m_keepGoing = true;
	private:
		std::thread m_thread;
	};
}