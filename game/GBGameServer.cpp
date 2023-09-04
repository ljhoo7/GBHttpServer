#include "stdafx.h"
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
			Gather(pEol, transferredBytes, messageID);
		}
		break;
		case VectoredIO::STATE::LENGTH:
		{
			Gather(pEol, transferredBytes, lengthToReceive);
		}
		break;
		case VectoredIO::STATE::PAYLOAD:
		{
			// TO DO
			// make fbb
			// get fbb.buffer
			char* fbbBuffer = 0;
			if (Gather(pEol, transferredBytes, fbbBuffer))
			{
				const auto m_pHandler = m_handlers.find(messageID);
				if (m_pHandler == m_handlers.end())
				{
					ErrorLog(std::format("receive packet handler not found. - messageID : {}", messageID));
					return false;
				}

				const auto [succeeded, fbb] = m_pHandler->second->CallResHandler();
				if (!succeeded)
				{
					return false;
				}
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

		const auto [succeeded, fbb] = m_pHandler->second->CallReqHandler();
		if (!succeeded)
		{
			return false;
		}

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