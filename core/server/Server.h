#pragma once

#include "Util.h"
#include "ExpandedOverlapped.h"
#include "../shared/PrivateShared.h"
#include "boost/thread/future.hpp"
#include "boost/thread/executors/executor.hpp"
#include "winsock2.h"
#include "MSWSock.h"

#include <thread>
#include <vector>
#include <string>
#include <atomic>
#include <unordered_map>
#include <limits>

namespace flatbuffers
{
	class Table;
}

namespace GenericBoson
{
	const int ISSUED_ACCEPTEX_COUNT = 100;// SOMAXCONN / sizeof(ExpandedOverlapped) / 200;

	class Server
	{
	public:
		Server() = default;
		Server(uint16_t portNum) : m_port(portNum) {}
		Server(const Server&) = default;

		std::string Start();

		virtual ~Server();

		bool GetKeepLooping() const
		{
			return m_keepLooping;
		}
	protected:
		virtual void OnConnected(ExpandedOverlapped* pEol) = 0;

		void Send(ExpandedOverlapped* pEol);

		//// \return true - all completed, false - not yet gathering completed.
		//virtual bool OnReceived(ExpandedOverlapped* pEol, DWORD receivedBytes) = 0;
		//virtual bool OnSent(ExpandedOverlapped* pEol, DWORD sentBytes) = 0;
		int IssueRecv(ExpandedOverlapped* pEol, ULONG lengthToReceive);
		int IssueSend(ExpandedOverlapped* pEol, const unsigned long throttling = 30);//(std::numeric_limits<unsigned long>::max)());
	private:
		std::pair<bool, std::string> SetListeningSocket();
		void SendThreadFunction();

		void ThreadFunction();
	private:
		PrivateShared m_CoreShared;

		int m_threadPoolSize = 0;
		std::vector<std::thread> m_threadPool;
		boost::future<void> m_sendTask;

		// #ToDo lock free circular queue
		std::mutex m_sendLock;
		std::unordered_map<SOCKET, std::queue<ExpandedOverlapped*>> m_sendQueues;
		std::vector<ExpandedOverlapped> m_sessions;

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