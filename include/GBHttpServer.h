#pragma once

#include <tuple>
#include <functional>
#include <thread>
#include <mutex>

#include "GBHttpRouter.h"
#include "GBMethod.h"
#include "GBGETMethod.h"
#include "GBPathSegment.h"
#include "GBHttpRequestLineReader.h"
#include "GBHttpRouter.h"
#include "GBExpandedOverlapped.h"
#include "GBHttpHeaderReader.h"

namespace GenericBoson
{
	const int ISSUED_ACCEPTEX_COUNT = 100;// SOMAXCONN / sizeof(ExpandedOverlapped) / 200;

	class GBHttpServer
	{
		int m_threadPoolSize = 0;
		std::vector<std::thread> m_threadPool;
		std::vector<GBExpandedOverlapped> m_sessions;

		WSADATA m_wsaData;
		sockaddr_in m_addr, m_client;

		SOCKET m_listeningSocket;

		// AcceptEx 함수 포인터
		LPFN_ACCEPTEX m_lpfnAcceptEx = NULL;

		uint16_t m_port = 0;

		int m_addrSize = sizeof(sockaddr_in);

		// 주의 : 실제 사용은 안하지만 있어야 제대로 동작한다.
		char m_listenBuffer[1024] = { 0, };

		//
		// \param pathTree
		// \param pTargetPath
		bool TraversePathTree(const std::vector<std::string>& pathTree, PathSegment*& pTargetPath);

		std::pair<bool, std::string> SetListeningSocket();
		std::string GetWSALastErrorString();
		std::string GetWSALastErrorString(int lastError);

		// \return true - all completed, false - not yet gathering completed.
		static bool OnReceived(GBExpandedOverlapped* pEol, DWORD receivedBytes);

		static int IssueRecv(GBExpandedOverlapped* pEol, ULONG lengthToReceive);
		static int IssueSend(GBExpandedOverlapped* pEol);

		static std::mutex g_mainCriticalsection;

		// Equivalent to '/'
		static PathSegment g_rootPath;
		static std::unique_ptr<GBHttpRouterBase> g_pRouter;

		static HANDLE g_IOCP;
		static volatile bool g_keepLooping;
		static void ThreadFunction();
	public:
		GBHttpServer() : GBHttpServer(8000) {};
		GBHttpServer(uint16_t portNum) : m_port(portNum) {};
		virtual ~GBHttpServer();

		bool GET(const std::string_view targetPath, const std::function<void(int)>& func);
		bool HEAD(const std::string_view targetPath, const std::function<void(int)>& func);
		bool POST(const std::string_view targetPath, const std::function<void(int)>& func);

		std::pair<bool, std::string> Start();
	};
}