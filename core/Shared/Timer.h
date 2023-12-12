#pragma once

#include "ProcessExitObserver.h"
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
		ITimer(const int64_t periodNs) 
			: m_PeriodNs(periodNs) 
		{}

		virtual ~ITimer() = default;

		virtual void OnTime() = 0;

		int64_t PeriodMs() const
		{
			return m_PeriodNs;
		}
	private:
		int64_t m_PeriodNs;
	};

	struct TimerComparer
	{
		bool operator()(const std::shared_ptr<ITimer>& lhs, const std::shared_ptr<ITimer>& rhs) { return lhs->PeriodMs() < rhs->PeriodMs(); };
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

		void AddTimer(const std::shared_ptr<ITimer>& pTimer)
		{
			std::lock_guard<std::shared_mutex> lock(m_Lock);
			m_Timers.push(pTimer);
		}

		std::shared_ptr<ITimer> GetFirstTimer()
		{
			std::shared_lock<std::shared_mutex> lock(m_Lock);

			if (m_Timers.empty())
			{
				return nullptr;
			}

			const auto pFirst = m_Timers.top();
			m_Timers.pop();

			return pFirst;
		}

	private:
		void OnStart()
		{
			while (ProcessExitSubject::GetInstance()->KeepGoing())
			{
				const auto pFirst = GetFirstTimer();

				std::this_thread::sleep_for(std::chrono::milliseconds(pFirst->PeriodMs()));

				if (!pFirst)
				{
					continue;
				}

				pFirst->OnTime();
			}
		}
		
	public:
	private:
		std::thread m_Thread;

		static const int64_t DEFAULT_WAIT_TIME_MS = 10;

		mutable std::shared_mutex m_Lock;
		std::priority_queue<std::shared_ptr<ITimer>, std::vector<std::shared_ptr<ITimer>>, TimerComparer> m_Timers;
	};
}