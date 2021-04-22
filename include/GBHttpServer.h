#pragma once

#include <functional>

#include "GBHttpRouter.h"
#include "GBMethod.h"
#include "GBGETMethod.h"

namespace GenericBoson
{
	class GBHttpServer
	{
		WSADATA m_wsaData;
		SOCKET m_listeningSocket;
		sockaddr_in m_addr, m_client;

		char m_buffer[1024];

		std::map<std::string, PathSegment> m_methodTree;
		std::unique_ptr<GBHttpRouterBase> m_pRouter = nullptr;

		int m_addrSize = sizeof(sockaddr_in);
	public:
		GBHttpServer() : GBHttpServer(8000) {};
		GBHttpServer(uint16_t portNum);
		virtual ~GBHttpServer();

		bool GET(const std::string_view targetPath, const std::function<void(int)>& func);

		bool Start();
	};
}