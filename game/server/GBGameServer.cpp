#include "stdafx.h"

#define NOMINMAX

#include "GBGameServer.h"
#include "../../core/server/GBExpandedOverlapped.h"


namespace GenericBoson
{
	bool GBGameServer::OnReceived(GBExpandedOverlapped* pEol, const DWORD transferredBytes)
	{
		static BUFFER_SIZE_TYPE messageID = 0;

		switch (pEol->m_gatherInput.GetState())
		{
		case VectoredIO::STATE::ID:
		{
			if (Gather(pEol->m_gatherInput, transferredBytes))
			{
				messageID = reinterpret_cast<decltype(messageID)>(pEol->m_gatherInput.m_buffer);
				pEol->m_gatherInput.m_length = sizeof(pEol->m_gatherInput.m_length);
			}
		}
		break;
		case VectoredIO::STATE::LENGTH:
		{
			if (Gather(pEol->m_gatherInput, transferredBytes))
			{
				pEol->m_gatherInput.m_length = reinterpret_cast<BUFFER_SIZE_TYPE>(pEol->m_gatherInput.m_buffer);
			}
		}
		break;
		case VectoredIO::STATE::PAYLOAD:
		{
			if (Gather(pEol->m_gatherInput, transferredBytes))
			{
				const auto m_pStub = m_stubs.find(messageID);
				if (m_pStub == m_stubs.end())
				{
					ErrorLog(std::format("receive packet handler not found. - messageID : {}", messageID));
					return false;
				}

				m_pStub->second->CallStub(pEol->m_gatherInput.m_buffer);
				pEol->m_gatherInput.m_length = sizeof(messageID);
			}
		}
		break;
		default:
		{
			assert(false);
		}
		break;
		}

		IssueRecv(pEol, pEol->m_gatherInput.m_length - pEol->m_gatherInput.m_offset);
		return true;
	}

	bool GBGameServer::OnSent(GBExpandedOverlapped* pEol, const DWORD transferredBytes)
	{
		return true;
	}

	bool GBGameServer::ErrorLog(const std::string_view msg)
	{
		return true;
	}

	bool GBGameServer::WarningLog(const std::string_view msg)
	{
		return true;
	}

	bool GBGameServer::InfoLog(const std::string_view msg)
	{
		return true;
	}

	void GBGameServer::OnConnected(GBExpandedOverlapped* pEol)
	{
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