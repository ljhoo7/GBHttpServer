#pragma once

#include "winsock2.h"

namespace GenericBoson
{
	struct GBExpandedOverlapped;

	class GBGameServer
	{
		bool OnReceived(const GBExpandedOverlapped* pEol, const DWORD transferredBytes);
		bool OnSent(const GBExpandedOverlapped* pEol, const DWORD transferredBytes);
	};
}