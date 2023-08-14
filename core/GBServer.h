#pragma once

#include "GBExpandedOverlapped.h"
#include "winsock2.h"
#include "MSWSock.h"

#include <thread>
#include <vector>
#include <string>
#include <atomic>

namespace GenericBoson
{
	const int ISSUED_ACCEPTEX_COUNT = 100;// SOMAXCONN / sizeof(ExpandedOverlapped) / 200;

	template<typename T>
	class GBServer
	{
	public:
		std::pair<bool, std::string> Start();

		GBServer() : m_port(8000) {};
		GBServer(uint16_t portNum) : m_port(portNum) {};
		~GBServer()
		{
			// winsock2 종료 처리
			closesocket(m_listeningSocket);
			WSACleanup();

			m_keepLooping = false;
		}

	private:
		
		std::pair<bool, std::string> SetListeningSocket();

		void ThreadFunction()
		{
			DWORD transferredBytes;
			u_long completionKey;
			GBExpandedOverlapped* pEol = nullptr;

			while (true == m_keepLooping)
			{
				BOOL result = GetQueuedCompletionStatus(g_IOCP, &transferredBytes, (PULONG_PTR)&completionKey, (OVERLAPPED**)&pEol, INFINITE);

				switch (pEol->m_type)
				{
				case IO_TYPE::ACCEPT:
				{
					int issueRecvResult = IssueRecv(pEol, BUFFER_SIZE);
					int lastError = WSAGetLastError();

					if (SOCKET_ERROR == issueRecvResult && WSA_IO_PENDING != lastError)
					{
						// #ToDo
						// Issue receiving failed.
					}
				}
				break;
				case IO_TYPE::RECEIVE:
				{
					bool ret = T.OnReceived(pEol, transferredBytes);
					if (false == ret)
					{
						continue;
					}
				}
				break;
				case IO_TYPE::SEND:
				{
					bool ret = T.OnSent(pEol, transferredBytes);

					// 소켓 닫기
					closesocket(pEol->m_socket);
				}
				break;
				}
			}
		}

		//// \return true - all completed, false - not yet gathering completed.
		//virtual bool OnReceived(GBExpandedOverlapped* pEol, DWORD receivedBytes) = 0;
		//virtual bool OnSent(GBExpandedOverlapped* pEol, DWORD sentBytes) = 0;

		static int IssueRecv(GBExpandedOverlapped* pEol, ULONG lengthToReceive);
	private:
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

		std::atomic_bool m_keepLooping = true;

		static HANDLE g_IOCP;
	};
}