#pragma once

#include "Singleton.h"

#include <thread>
#include <chrono>
#include <queue>
#include <shared_mutex>

namespace GenericBoson
{
	class ITimer
	{
	public:
		ITimer(const int64_t startTimeNs, const int64_t periodNs) 
			: m_PeriodNs(periodNs) 
		{}

		virtual ~ITimer() = default;

		virtual void OnTime() = 0;

		int64_t PeriodNs() const
		{
			return m_PeriodNs;
		}
	private:
		int64_t m_PeriodNs;
	};

	struct TimerComparer
	{
		bool operator()(const std::shared_ptr<ITimer>& lhs, const std::shared_ptr<ITimer>& rhs) { return lhs->PeriodNs() < rhs->PeriodNs(); };
	};

	class TimerManager : public Singleton<TimerManager>
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

		bool AddTimer(const std::shared_ptr<ITimer>& pTimer)
		{
			std::lock_guard<std::shared_mutex> lock(m_Lock);
			m_Timers.push(pTimer);
		}

		std::pair<std::shared_ptr<ITimer>, int64_t> GetFirstTimer()
		{
			std::shared_lock<std::shared_mutex> lock(m_Lock);

			if (m_Timers.empty())
			{
				return { nullptr, DEFAULT_WAIT_TIME_NS };
			}

			const auto pFirst = m_Timers.top();
			m_Timers.pop();

			if (m_Timers.empty())
			{
				return { pFirst, DEFAULT_WAIT_TIME_NS };
			}

			return { pFirst, m_Timers.top()->PeriodNs() };
		}

	private:
		void OnStart()
		{
			while (m_KeepGoing)
			{
				const auto& [pFirst, waitTimeNs] = GetFirstTimer();

				std::this_thread::sleep_for(std::chrono::nanoseconds(waitTimeNs));

				if (!pFirst)
				{
					continue;
				}

				pFirst->OnTime();
			}
		}
		
	public:
		std::atomic_bool m_KeepGoing = true;
	private:
		std::thread m_Thread;

		static const int64_t DEFAULT_WAIT_TIME_NS = 10;

		mutable std::shared_mutex m_Lock;
		std::priority_queue<std::shared_ptr<ITimer>, std::vector<std::shared_ptr<ITimer>>, TimerComparer> m_Timers;
	};
}