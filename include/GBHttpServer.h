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

const int BUFFER_SIZE = 4096;
const int ISSUED_ACCEPTEX_COUNT = 100;// SOMAXCONN / sizeof(ExpandedOverlapped) / 200;

namespace GenericBoson
{
	class GBHttpServer
	{
		enum class IO_TYPE : int64_t
		{
			ACCEPT = 1,
			RECEIVE,
			SEND,
		};

		struct GBBuffer
		{
			char m_buffer[BUFFER_SIZE] = { 0, };
			int m_writeOffset = 0;
			int m_readOffset = 0;
		};

		struct ExpandedOverlapped : public WSAOVERLAPPED
		{
			SOCKET m_socket = INVALID_SOCKET;
			IO_TYPE m_type = IO_TYPE::ACCEPT;
			uint32_t m_leftBytesToReceive = 0;

			// #ToDo
			// This must be exchanged with a circular lock-free queue.
			GBBuffer m_receiveBuffer;
			GBBuffer m_writeBuffer;
		};

		int m_threadPoolSize = 0;
		std::vector<std::thread> m_threadPool;
		std::vector<ExpandedOverlapped> m_sessions;

		WSADATA m_wsaData;
		sockaddr_in m_addr, m_client;

		SOCKET m_listeningSocket;

		// AcceptEx 함수 포인터
		LPFN_ACCEPTEX m_lpfnAcceptEx = NULL;

		uint16_t m_port = 0;

		int m_addrSize = sizeof(sockaddr_in);

		// 주의 : 실제 사용은 안하지만 있어야 제대로 동작한다.
		char m_listenBuffer[BUFFER_SIZE] = { 0, };

		//
		// \param pathTree
		// \param pTargetPath
		bool TraversePathTree(const std::vector<std::string>& pathTree, PathSegment*& pTargetPath);

		std::pair<bool, std::string> SetListeningSocket();
		std::string GetWSALastErrorString();
		std::string GetWSALastErrorString(int lastError);

		static int IssueRecv(ExpandedOverlapped* pEol, ULONG lengthToReceive);

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