#pragma once

#include <tuple>
#include <functional>

#include "GBHttpRouter.h"
#include "GBMethod.h"
#include "GBGETMethod.h"
#include "GBPathSegment.h"
#include "GBHttpRequestLineReader.h"
#include "GBHttpRouter.h"

namespace GenericBoson
{
	class GBHttpServer
	{
		WSADATA m_wsaData;
		sockaddr_in m_addr, m_client;

		HANDLE m_IOCP;
		SOCKET m_listeningSocket;

		char m_buffer[1024];

		uint16_t m_port = 0;

		// Equivalent to '/'
		PathSegment m_rootPath;
		std::unique_ptr<GBHttpRouterBase> m_pRouter = nullptr;

		int m_addrSize = sizeof(sockaddr_in);

		//
		// \param pathTree
		// \param pTargetPath
		bool TraversePathTree(const std::vector<std::string>& pathTree, PathSegment*& pTargetPath);

		std::pair<bool, std::string> SetListeningSocket();
	public:
		GBHttpServer() : GBHttpServer(8000) {};
		GBHttpServer(uint16_t portNum) : m_port(portNum) {};
		virtual ~GBHttpServer();

		bool GET(const std::string_view targetPath, const std::function<void(int)>& func);
		bool HEAD(const std::string_view targetPath, const std::function<void(int)>& func);
		bool POST(const std::string_view targetPath, const std::function<void(int)>& func);

		bool Start();
	};
}