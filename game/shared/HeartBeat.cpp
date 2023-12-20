#include "../../core/Shared/TimerManager.h"
#include "../server/GameServer.h"
#include "HeartBeat.h"

namespace GenericBoson
{
	void HeartBeat::OnTime()
	{
		auto pOwner = m_wpOwner.lock();
		if (!pOwner)
		{
			return;
		}
		
		pOwner->

		TimerManager::GetInstance()->AddTimer(shared_from_this());
	}
}

