#pragma once

#include "Singleton.h"

#include <vector>
#include <atomic>
#include <shared_mutex>

namespace GenericBoson
{
	class ProcessExitSubject : public Singleton<ProcessExitSubject>
	{
	public:
		ProcessExitSubject() = default;
		virtual ~ProcessExitSubject()
		{
			m_KeepGoing.store(false);
		}

		bool KeepGoing() const
		{
			return m_KeepGoing.load();
		}
	private:
		std::atomic_bool m_KeepGoing = true;
	};
}