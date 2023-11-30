#pragma once

#include <thread>
#include <chrono>

namespace GenericBoson
{
	class ITimer
	{

	};

	class Timer
	{
	public:
		Timer()
			: m_thread(&Timer::OnTimerStart, this)
		{
		}

		~Timer()
		{
			if (m_thread.joinable())
			{
				m_thread.join();
			}
		}

		virtual void OnTime() = 0;
	private:
		void OnTimerStart()
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