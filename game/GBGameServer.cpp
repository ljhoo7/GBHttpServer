#include "stdafx.h"
#include "GBGameServer.h"

#include "../core/GBExpandedOverlapped.h"


namespace GenericBoson
{
	bool GBGameServer::OnReceived(GBExpandedOverlapped* pEol, const DWORD transferredBytes)
	{
		ULONG lengthToReceive = 0;

		switch (pEol->m_gatherState)
		{
		case GBExpandedOverlapped::STATE::ID:
		{
			if (pEol->m_length < pEol->m_recvOffset + transferredBytes)
			{

				pEol->m_gatherState = GBExpandedOverlapped::STATE::LENGTH;
			}
		}
		break;
		case GBExpandedOverlapped::STATE::LENGTH:
		{
			if (pEol->m_length < pEol->m_recvOffset + transferredBytes)
			{
				lengthToReceive = reinterpret_cast<BUFFER_TYPE>(pEol->m_pRecvBuffer);
				pEol->m_gatherState = GBExpandedOverlapped::STATE::PAYLOAD;
			}
		}
		break;
		case GBExpandedOverlapped::STATE::PAYLOAD:
		{
			if (pEol->m_length < pEol->m_recvOffset + transferredBytes)
			{

				pEol->m_gatherState = GBExpandedOverlapped::STATE::ID;
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
		eol.m_pSendBuffer = reinterpret_cast<char*>(fbb.GetBufferPointer());
		eol.m_sendOffset += fbb.GetSize();

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