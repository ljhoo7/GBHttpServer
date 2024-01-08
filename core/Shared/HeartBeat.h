#pragma once

#include "../../core/Shared/TimerManager.h"

#include <memory>

namespace GenericBoson
{
	class CoreShared;

	class HeartBeat : public ITimer, public std::enable_shared_from_this<HeartBeat>
	{
	public:
		HeartBeat(const int64_t periodMs) : ITimer(periodMs) {}
		void OnTime() override;
	private:
		//std::weak_ptr<CoreShared> m_wpOwner;
	};
}