#pragma once

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

		int m_addrSize = sizeof(sockaddr_in);
	public:
		GBHttpServer() : GBHttpServer(8000) {};
		GBHttpServer(uint16_t portNum);
		virtual ~GBHttpServer();

		void Add(std::function<void(int)> callable)
		{
		}

		bool Start();
	};
}