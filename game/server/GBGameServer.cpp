#include "stdafx.h"

#define NOMINMAX

#include "GBGameServer.h"
#include "../../core/server/GBExpandedOverlapped.h"


namespace GenericBoson
{
	bool GBGameServer::OnReceived(VectoredIO& inputData, const DWORD transferredBytes)
	{
		static BUFFER_SIZE_TYPE messageID = 0;

		switch (inputData.GetState())
		{
		case VectoredIO::STATE::ID:
		{
			if (Gather(inputData, transferredBytes))
			{
				messageID = reinterpret_cast<decltype(messageID)>(inputData.m_buffer);
				inputData.m_length = sizeof(inputData.m_length);
			}
		}
		break;
		case VectoredIO::STATE::LENGTH:
		{
			if (Gather(inputData, transferredBytes))
			{
				inputData.m_length = reinterpret_cast<BUFFER_SIZE_TYPE>(inputData.m_buffer);
			}
		}
		break;
		case VectoredIO::STATE::PAYLOAD:
		{
			if (Gather(inputData, transferredBytes))
			{
				const auto m_pStub = m_stubs.find(messageID);
				if (m_pStub == m_stubs.end())
				{
					ErrorLog(std::format("receive packet handler not found. - messageID : {}", messageID));
					return false;
				}

				m_pStub->second->CallStub(inputData.m_buffer);
				inputData.m_length = sizeof(messageID);
			}
		}
		break;
		default:
		{
			assert(false);
		}
		break;
		}

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