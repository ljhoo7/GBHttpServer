#include "stdafx.h"

#define NOMINMAX

#include "../shared/HeartBeat.h"
#include "../../core/Shared/TimerManager.h"

#include "GameServer.h"
#include "../../core/server/ExpandedOverlapped.h"


namespace GenericBoson
{
	void GBGameServer::OnConnected(GBExpandedOverlapped* pEol)
	{
		const auto pTimer = std::make_shared<HeartBeat>(1000);
		TimerManager::GetInstance()->AddTimer(pTimer);

		if (m_connectedTask)
		{
			m_connectedTask(pEol);
		}
	}

	void GBGameServer::SetConnectedTask(
		const std::function<void(GBExpandedOverlapped* pEol)>& task
	)
	{
		m_connectedTask = task;
	}

	ThreadSafeBufferAllocator GBGameServer::g_bufferAllocator;
}