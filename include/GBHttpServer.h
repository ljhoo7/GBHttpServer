#pragma once

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
		SOCKET m_listeningSocket;
		sockaddr_in m_addr, m_client;

		char m_buffer[1024];

		// Equivalent to '/'
		PathSegment m_rootPath;
		std::unique_ptr<GBHttpRouterBase> m_pRouter = nullptr;

		int m_addrSize = sizeof(sockaddr_in);

		//
		// \param pathTree
		// \param pTargetPath
		bool TraversePathTree(const std::vector<std::string>& pathTree, PathSegment* pTargetPath);
	public:
		GBHttpServer() : GBHttpServer(8000) {};
		GBHttpServer(uint16_t portNum);
		virtual ~GBHttpServer();

		bool GET(const std::string_view targetPath, const std::function<void(int)>& func);
		bool HEAD(const std::string_view targetPath, const std::function<void(int)>& func);
		bool POST(const std::string_view targetPath, const std::function<void(int)>& func);

		bool Start();
	};
}