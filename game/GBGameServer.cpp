#include "stdafx.h"
#include "GBGameServer.h"

#include "../core/GBExpandedOverlapped.h"
#include "flatbuffers/flatbuffers.h"

namespace GenericBoson
{
	bool GBGameServer::OnReceive(const GBExpandedOverlapped* pEol, const DWORD transferredBytes)
	{


		return true;
	}

	bool GBGameServer::OnSend(GBExpandedOverlapped* pEol, const DWORD transferredBytes)
	{
		::flatbuffers::FlatBufferBuilder fbb;

		//fbb.

		pEol->m_pRecvBuffer = reinterpret_cast<char*>(fbb.GetBufferPointer());

		pEol->m_sendOffset += fbb.GetSize();

		return true;
	}
}