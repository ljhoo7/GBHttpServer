#pragma once

#include "ProcessExitObserver.h"
#include "Singleton.h"

#include <thread>
#include <chrono>
#include <queue>
#include <shared_mutex>

namespace GenericBoson
{
	class ITimer;

	class TimerManager : public Singleton<TimerManager>
	{
	public:
		TimerManager();
		~TimerManager();

		void AddTimer(const std::shared_ptr<ITimer>& pTimer);

		std::shared_ptr<ITimer> GetFirstTimer();

	private:
		void OnStart();
		
	public:
	private:
		std::thread m_Thread;

		static const int64_t DEFAULT_WAIT_TIME_MS = 10;

		mutable std::shared_mutex m_Lock;
		std::priority_queue<std::shared_ptr<ITimer>, std::vector<std::shared_ptr<ITimer>>, TimerComparer> m_Timers;
	};
}