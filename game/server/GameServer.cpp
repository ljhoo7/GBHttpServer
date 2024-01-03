#include "stdafx.h"

#define NOMINMAX

#include "GameServer.h"

#include "../../core/server/ExpandedOverlapped.h"

#include "../shared/HeartBeat.h"
#include "../flatbufferschema/internal_generated.h"
#include "../../core/Shared/TimerManager.h"


namespace GenericBoson
{
	void PongStub(const GameInternal::PingPong& pingPong)
	{
	}

	void GameServer::OnConnected(ExpandedOverlapped* pEol)
	{
		AddStub(1, PongStub);

		const auto pTimer = std::make_shared<HeartBeat>(1000);
		TimerManager::GetInstance()->AddTimer(pTimer);

		if (m_connectedTask)
		{
			m_connectedTask(pEol);
		}
	}

	void GameServer::SendPing(ExpandedOverlapped* pEol)
	{
		
	}

	void GameServer::SetConnectedTask(
		const std::function<void(ExpandedOverlapped* pEol)>& task
	)
	{
		m_connectedTask = task;
	}

	ThreadSafeBufferAllocator GameServer::g_bufferAllocator;
}