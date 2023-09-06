#include "stdafx.h"

#define NOMINMAX

#include "GBGameServer.h"
#include "../core/GBExpandedOverlapped.h"


namespace GenericBoson
{
	bool GBGameServer::OnReceived(GBExpandedOverlapped* pEol, const DWORD transferredBytes)
	{
		static ULONG messageID = 0, lengthToReceive = 0;

		switch (pEol->m_gatherInput.GetState())
		{
		case VectoredIO::STATE::ID:
		{
			if (Gather(pEol, transferredBytes))
			{
				messageID = reinterpret_cast<ULONG>(pEol->m_gatherInput.m_pBuffer);
				pEol->m_gatherInput.m_length = sizeof(pEol->m_gatherInput.m_length);
			}
		}
		break;
		case VectoredIO::STATE::LENGTH:
		{
			if (Gather(pEol, transferredBytes))
			{
				pEol->m_gatherInput.m_length = reinterpret_cast<BUFFER_SIZE_TYPE>(pEol->m_gatherInput.m_pBuffer);
			}
		}
		break;
		case VectoredIO::STATE::PAYLOAD:
		{
			if (Gather(pEol, transferredBytes))
			{
				const auto m_pHandler = m_handlers.find(messageID);
				if (m_pHandler == m_handlers.end())
				{
					ErrorLog(std::format("receive packet handler not found. - messageID : {}", messageID));
					return false;
				}

				const auto succeeded = m_pHandler->second->CallResHandler(pEol->m_gatherInput.m_pBuffer);
				if (!succeeded)
				{
					return false;
				}

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

		IssueRecv(pEol, lengthToReceive);
		return true;
	}

	bool GBGameServer::Send(const int messageID)
	{
		const auto m_pHandler = m_handlers.find(messageID);
		if (m_pHandler == m_handlers.end())
		{
			ErrorLog(std::format("send packet handler not found. - messageID : {}", messageID));
			return false;
		}

		const auto opFbb = m_pHandler->second->CallReqHandler();
		if (!opFbb.has_value())
		{
			return false;
		}

		auto& fbb = opFbb.value();

		GBExpandedOverlapped eol;
		eol.m_scatterOutput.m_pBuffer = reinterpret_cast<char*>(fbb.GetBufferPointer());
		eol.m_scatterOutput.m_offset += fbb.GetSize();

		return __super::Send(&eol);
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
}