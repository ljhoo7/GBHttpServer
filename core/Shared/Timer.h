#pragma once

#include <thread>
#include <chrono>
#include <queue>
#include <shared_mutex>

namespace GenericBoson
{
	struct TimerComparer
	{
		bool operator()(const ITimer& lhs, const ITimer& rhs) { return lhs.StartTimeMs() < rhs.StartTimeMs() };
	};

	class ITimer
	{
	public:
		ITimer(const int64_t startTimeMs, const int64_t periodMs) 
			: m_StartTimeMs(startTimeMs), m_PeriodMs(periodMs) 
		{}

		virtual void OnTime() = 0;

		int64_t StartTimeMs() const
		{
			return m_StartTimeMs;
		}

		int64_t PeriodMs() const
		{
			return m_StartTimeMs;
		}
	private:
		int64_t m_StartTimeMs, m_PeriodMs;
	};

	class TimerManager
	{
	public:
		TimerManager()
			: m_Thread(&TimerManager::OnStart, this)
		{
		}

		~TimerManager()
		{
			if (m_Thread.joinable())
			{
				m_Thread.join();
			}
		}

		bool AddTimer(const ITimer& timer)
		{
			std::lock_guard<std::shared_mutex> lock(m_Lock);
			m_Timers.push(timer);
		}

		std::pair<ITimer, int64_t> GetTimer() const
		{
			std::shared_lock<std::shared_mutex> lock(m_Lock);
		}

	private:
		void OnStart()
		{
			const auto startTime = std::chrono::steady_clock::now();

			while (m_KeepGoing)
			{
				
			}
		}
		
	public:
		std::atomic_bool m_KeepGoing = true;
	private:
		std::thread m_Thread;

		mutable std::shared_mutex m_Lock;
		std::priority_queue<ITimer, std::vector<ITimer>, TimerComparer> m_Timers;
	};
}