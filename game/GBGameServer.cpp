#include "stdafx.h"
#include "GBGameServer.h"

#include "../core/GBExpandedOverlapped.h"
#include "flatbuffers/flatbuffers.h"

namespace GenericBoson
{
	bool GBGameServer::OnReceived(const GBExpandedOverlapped* pEol, const DWORD transferredBytes)
	{
		::flatbuffers::FlatBufferBuilder fbb;
		
		return true;
	}

	bool GBGameServer::OnSent(const GBExpandedOverlapped* pEol, const DWORD transferredBytes)
	{

		return true;
	}
}