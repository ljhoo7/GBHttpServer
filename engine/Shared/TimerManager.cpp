#include "TimerManager.h"

#include <chrono>

namespace GenericBoson
{
	TimerManager::TimerManager()
		: m_Thread(&TimerManager::OnStart, this)
	{
	}

	TimerManager::~TimerManager()
	{
		if (m_Thread.joinable())
		{
			m_Thread.join();
		}
	}

	void TimerManager::AddTimer(const std::shared_ptr<ITimer>& pTimer)
	{
		std::lock_guard<std::shared_mutex> lock(m_Lock);
		m_Timers.push(pTimer);
	}

	std::shared_ptr<ITimer> TimerManager::GetFirstTimer()
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

	void TimerManager::OnStart()
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
}