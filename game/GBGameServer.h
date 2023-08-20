#pragma once

#include "../core/GBServer.h"
#include "winsock2.h"

namespace GenericBoson
{
	struct GBExpandedOverlapped;

	class GBGameServer : public GBServer
	{
	public:
		GBGameServer(uint16_t portNum) : GBServer(portNum) {}
		virtual ~GBGameServer() = default;

	private:
		bool OnReceive(const GBExpandedOverlapped* pEol, const DWORD transferredBytes) override;
		bool OnSend(GBExpandedOverlapped* pEol, const DWORD transferredBytes) override;
	};
}