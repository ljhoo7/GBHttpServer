#pragma once

#include "GBUtil.h"
#include "GBExpandedOverlapped.h"
#include "boost/thread/future.hpp"
#include "boost/thread/executors/executor.hpp"
#include "winsock2.h"
#include "MSWSock.h"

#include <thread>
#include <vector>
#include <string>
#include <atomic>
#include <unordered_map>

namespace flatbuffers
{
	class Table;
}

namespace GenericBoson
{
	const int ISSUED_ACCEPTEX_COUNT = 100;// SOMAXCONN / sizeof(ExpandedOverlapped) / 200;

	class GBServer
	{
	public:
		std::pair<bool, std::string> Start();

		GBServer(uint16_t portNum) : m_port(portNum) {};
		virtual ~GBServer();

	protected:
		virtual bool OnReceived(const GBExpandedOverlapped* pEol, const DWORD transferredBytes) = 0;
		virtual bool OnSent(GBExpandedOverlapped* pEol, const DWORD transferredBytes) = 0;

		virtual bool ErrorLog(const std::string_view msg) = 0;
		virtual bool WarningLog(const std::string_view msg) = 0;
		virtual bool InfoLog(const std::string_view msg) = 0;

		bool Send(GBExpandedOverlapped* pEol);

		//// \return true - all completed, false - not yet gathering completed.
		//virtual bool OnReceived(GBExpandedOverlapped* pEol, DWORD receivedBytes) = 0;
		//virtual bool OnSent(GBExpandedOverlapped* pEol, DWORD sentBytes) = 0;
		int IssueRecv(GBExpandedOverlapped* pEol, ULONG lengthToReceive);
		int IssueSend(GBExpandedOverlapped* pEol);
	private:
		std::pair<bool, std::string> SetListeningSocket();
		void SendThreadFunction();

		void ThreadFunction();
	private:
		int m_threadPoolSize = 0;
		std::vector<std::thread> m_threadPool;
		boost::future<void> m_sendTask;

		std::unordered_map<SOCKET, boost::lockfree::queue<GBExpandedOverlapped*>> m_sendQueues;
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

		std::atomic_bool m_keepLooping = true;

		HANDLE m_IOCP = INVALID_HANDLE_VALUE;
	};
}