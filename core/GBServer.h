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
		std::pair<bool, std::string> Start()
		{
			bool result;
			std::string errorMsg;
			std::tie(result, errorMsg) = SetListeningSocket();

			m_sessions.resize(ISSUED_ACCEPTEX_COUNT);

			// AcceptEx 이슈
			for (int k = 0; k < ISSUED_ACCEPTEX_COUNT; ++k)
			{
				// AcceptEx 소켓만들기
				m_sessions[k].m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
				if (INVALID_SOCKET == m_sessions[k].m_socket)
				{
					return { false, GetWSALastErrorString() };
				}

				m_sessions[k].m_type = IO_TYPE::ACCEPT;

				// Posting an accept operation.
				DWORD returnedBytes;
				BOOL result = m_lpfnAcceptEx(m_listeningSocket, m_sessions[k].m_socket, m_listenBuffer, 0,
					sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16,
					&returnedBytes, &m_sessions[k]);
				int lastSocketError = WSAGetLastError();
				if (FALSE == result && ERROR_IO_PENDING != lastSocketError)
				{
					return { false, GetWSALastErrorString(lastSocketError) };
				}

				// Associate this accept socket withd IOCP.
				HANDLE associateAcceptSocketResult = CreateIoCompletionPort((HANDLE)m_sessions[k].m_socket, g_IOCP, (u_long)0, 0);
				if (NULL == associateAcceptSocketResult)
				{
					return { false, GetWSALastErrorString() };
				}
			}

			return { true, {} };
		}

		GBServer(uint16_t portNum) : m_port(portNum) {};
		~GBServer()
		{
			// winsock2 종료 처리
			closesocket(m_listeningSocket);
			WSACleanup();

			m_keepLooping = false;
		}

	private:
		std::pair<bool, std::string> SetListeningSocket()
		{
#pragma region [1] Prepare and start listening port and IOCP
			// [1] - 1. WinSock 2.2 초기화
			if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &m_wsaData))
			{
				return { false, "WSAStartup failed\n" };
			}

			// [1] - 2.  IOCP 커널 오브젝트 만들기.
			g_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (u_long)0, 0);
			if (NULL == g_IOCP)
			{
				return { false, GetWSALastErrorString() };
			}

			m_threadPoolSize = 2 * std::thread::hardware_concurrency();
			for (int k = 0; k < m_threadPoolSize; ++k)
			{
				m_threadPool.emplace_back(&GBServer::ThreadFunction<>, this);
			}

			// [1] - 3.  소켓 만들기
			m_listeningSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
			if (INVALID_SOCKET == m_listeningSocket)
			{
				return { false, GetWSALastErrorString() };
			}

			// [1] - 4.  Associate the listening socket with the IOCP.
			HANDLE ret1 = CreateIoCompletionPort((HANDLE)m_listeningSocket, g_IOCP, (u_long)0, 0);

			if (NULL == ret1)
			{
				return { false, GetWSALastErrorString() };
			}

			// [1] - 5.  소켓 설정
			m_addr.sin_family = AF_INET;
			m_addr.sin_port = htons(m_port);
			m_addr.sin_addr.S_un.S_addr = INADDR_ANY;

			// [1] - 6.  소켓 바인드
			int ret2 = bind(m_listeningSocket, (struct sockaddr*)&m_addr, sizeof(m_addr));
			if (SOCKET_ERROR == ret2)
			{
				return { false, GetWSALastErrorString() };
			}

			// [1] - 7.  리스닝 포트 가동
			ret2 = listen(m_listeningSocket, SOMAXCONN);
			if (SOCKET_ERROR == ret2)
			{
				return { false, GetWSALastErrorString() };
			}
#pragma endregion [1] Prepare and start listening port and IOCP

#pragma region [2] Prepare AcceptEx and associate accept I/O requests to IOCP
			GUID GuidAcceptEx = WSAID_ACCEPTEX;
			DWORD returnedBytes;

			// [2] - 1. AcceptEx 함수 가져오기
			ret2 = WSAIoctl(m_listeningSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&GuidAcceptEx, sizeof(GuidAcceptEx),
				&m_lpfnAcceptEx, sizeof(m_lpfnAcceptEx),
				&returnedBytes, NULL, NULL);
			if (SOCKET_ERROR == ret2)
			{
				return { false, GetWSALastErrorString() };
			}
#pragma endregion [2] Prepare AcceptEx and associate accept I/O requests to IOCP

			return { true, {} };
		}

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
		static int IssueRecv(GBExpandedOverlapped* pEol, ULONG lengthToReceive)
		{
			pEol->m_type = IO_TYPE::RECEIVE;
			DWORD flag = 0;
			WSABUF wsaBuffer;
			wsaBuffer.len = lengthToReceive;			// packet length is 1 byte.
			wsaBuffer.buf = &pEol->m_recvBuffer[pEol->m_recvOffset];
			int recvResult = WSARecv(pEol->m_socket, &wsaBuffer, 1, nullptr, &flag, pEol, nullptr);

			return recvResult;
		}
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

		HANDLE m_IOCP = INVALID_HANDLE_VALUE;
	};
}