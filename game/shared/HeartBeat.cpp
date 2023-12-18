#include "../../core/Shared/TimerManager.h"
#include "HeartBeat.h"

namespace GenericBoson
{
	void HeartBeat::OnTime()
	{
		TimerManager::GetInstance()->AddTimer(shared_from_this());
	}
}

