#include "stdafx.h"
#include "GBGameServer.h"

#include "../core/GBExpandedOverlapped.h"
#include "flatbuffers/flatbuffers.h"

namespace GenericBoson
{
	bool GBGameServer::OnReceived(const GBExpandedOverlapped* pEol, const DWORD transferredBytes)
	{


		return true;
	}

	bool GBGameServer::Send(const int messageID, const std::shared_ptr<::flatbuffers::Table>& pMessage)
	{
		::flatbuffers::FlatBufferBuilder fbb;

		//fbb.

		GBExpandedOverlapped eol;
		eol.m_pReceiveBuffer = reinterpret_cast<char*>(fbb.GetBufferPointer());
		eol.m_receiveOffset += fbb.GetSize();

		__super::Send(&eol);

		return true;
	}

	bool GBGameServer::OnSent(GBExpandedOverlapped* pEol, const DWORD transferredBytes)
	{
		return true;
	}
}