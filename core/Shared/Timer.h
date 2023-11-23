#pragma once

#include <thread>
#include <chrono>

namespace GenericBoson
{
	class GBTimer
	{
	public:
		GBTimer()
			: m_thread(&GBTimer::OnTimerStart, this)
		{
		}

		~GBTimer()
		{
			if (m_thread.joinable())
			{
				m_thread.join();
			}
		}

		void OnTimerStart()
		{
		}

		virtual void OnTime() = 0;
	private:
		
		std::thread m_thread;
	};
}