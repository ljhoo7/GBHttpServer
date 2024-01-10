#include "TimerManager.h"
#include "HeartBeat.h"

namespace GenericBoson
{
	void HeartBeat::OnTime()
	{
		/*auto pOwner = m_wpOwner.lock();
		if (!pOwner)
		{
			return;
		}*/
		
		TimerManager::GetInstance()->AddTimer(shared_from_this());
	}
}

