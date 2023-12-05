#pragma once

#include "Singleton.h"

#include <thread>
#include <chrono>
#include <queue>
#include <shared_mutex>

namespace GenericBoson
{
	struct TimerComparer
	{
		bool operator()(const std::shared_ptr<ITimer>& lhs, const std::shared_ptr<ITimer>& rhs) { return lhs->StartTimeNs() < rhs->StartTimeNs(); };
	};

	class ITimer
	{
	public:
		ITimer(const int64_t startTimeNs, const int64_t periodNs) 
			: m_StartTimeNs(startTimeNs), m_PeriodNs(periodNs) 
		{}

		virtual ~ITimer() = default;

		virtual void OnTime() = 0;

		int64_t StartTimeNs() const
		{
			return m_StartTimeNs;
		}

		void StartTimeNs(const int64_t startTimeMs)
		{
			m_StartTimeNs = startTimeMs;
		}

		int64_t PeriodNs() const
		{
			return m_StartTimeNs;
		}
	private:
		int64_t m_StartTimeNs, m_PeriodNs;
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
				return { nullptr, 0 };
			}

			const auto pFirst = m_Timers.top();
			const int64_t currentTimeNs = std::chrono::steady_clock::now().time_since_epoch().count();
			if (currentTimeNs < pFirst->StartTimeNs())
			{
				return {nullptr, pFirst->StartTimeNs() - currentTimeNs};
			}

			// correction
			pFirst->StartTimeNs(currentTimeNs);

			m_Timers.pop();

			if (m_Timers.empty())
			{
				return { pFirst, pFirst->PeriodNs()};
			}

			const auto& pSecond = m_Timers.top();
			const auto intervalNs = pSecond->StartTimeNs() - pFirst->StartTimeNs();
			if (intervalNs < pFirst->PeriodNs())
			{
				return { pFirst, intervalNs };
			}

			return { pFirst, pFirst->PeriodNs() };
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

				pFirst->StartTimeNs(pFirst->StartTimeNs() + pFirst->PeriodNs());
				AddTimer(pFirst);
			}
		}
		
	public:
		std::atomic_bool m_KeepGoing = true;
	private:
		std::thread m_Thread;

		mutable std::shared_mutex m_Lock;
		std::priority_queue<std::shared_ptr<ITimer>, std::vector<std::shared_ptr<ITimer>>, TimerComparer> m_Timers;
	};
}