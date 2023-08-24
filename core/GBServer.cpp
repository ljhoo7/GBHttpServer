#include "stdafx.h"

#include "GBServer.h"

namespace GenericBoson
{
	std::pair<bool, std::string> GBServer::Start()
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
			HANDLE associateAcceptSocketResult = CreateIoCompletionPort((HANDLE)m_sessions[k].m_socket, m_IOCP, (u_long)0, 0);
			if (NULL == associateAcceptSocketResult)
			{
				return { false, GetWSALastErrorString() };
			}
		}

		return { true, {} };
	}

	GBServer::~GBServer()
	{
		using namespace std::chrono_literals;

		// winsock2 종료 처리
		closesocket(m_listeningSocket);
		WSACleanup();

		m_keepLooping = false;

		for (int k = 0; k < m_threadPoolSize; ++k)
		{
			if (!m_threadPool[k].joinable())
			{
				continue;
			}

			m_threadPool[k].join();
		}

		m_sendTask.get();

		while (!m_sendQueue.empty())
		{
			// preventing busy waiting
			std::this_thread::sleep_for(1ms);
		}
	}

	std::pair<bool, std::string> GBServer::SetListeningSocket()
	{
#pragma region [1] Prepare and start listening port and IOCP
		// [1] - 1. WinSock 2.2 초기화
		if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &m_wsaData))
		{
			return { false, "WSAStartup failed\n" };
		}

		// [1] - 2.  IOCP 커널 오브젝트 만들기.
		m_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (u_long)0, 0);
		if (NULL == m_IOCP)
		{
			return { false, GetWSALastErrorString() };
		}

		m_threadPoolSize = 2 * std::thread::hardware_concurrency() - 1; // 1 is send task
		for (int k = 0; k < m_threadPoolSize; ++k)
		{
			m_threadPool.emplace_back(&GBServer::ThreadFunction, this);
		}

		m_sendTask = boost::async(boost::bind(&GBServer::SendThreadFunction, this));

		// [1] - 3.  소켓 만들기
		m_listeningSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
		if (INVALID_SOCKET == m_listeningSocket)
		{
			return { false, GetWSALastErrorString() };
		}

		// [1] - 4.  Associate the listening socket with the IOCP.
		HANDLE ret1 = CreateIoCompletionPort((HANDLE)m_listeningSocket, m_IOCP, (u_long)0, 0);

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

	void GBServer::ThreadFunction()
	{
		DWORD transferredBytes;
		u_long completionKey;
		GBExpandedOverlapped* pEol = nullptr;

		while (true == m_keepLooping)
		{
			BOOL result = GetQueuedCompletionStatus(m_IOCP, &transferredBytes, (PULONG_PTR)&completionKey, (OVERLAPPED**)&pEol, INFINITE);

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
				bool ret = OnReceived(pEol, transferredBytes);
				if (false == ret)
				{
					continue;
				}

				//// 개더링이 끝나지 않았거나, 끝났어도 받은게 전혀없다면, 더 받으려고 한다.
				//if (true == gatheringNotFinished || true == gatheringFinishedButNothing)
				//{
				//	int issueRecvResult = IssueRecv(pEol, BUFFER_SIZE - pEol->m_offset);
				//	int lastError = WSAGetLastError();

				//	if (SOCKET_ERROR == issueRecvResult && WSA_IO_PENDING != lastError)
				//	{
				//		// #ToDo
				//		// Issue receiving failed.
				//	}

				//	return false;
				//}
			}
			break;
			case IO_TYPE::SEND:
			{
				bool ret = OnSent(pEol, transferredBytes);

				// 소켓 닫기
				//closesocket(pEol->m_socket);
			}
			break;
			}
		}
	}

	int GBServer::IssueRecv(GBExpandedOverlapped* pEol, ULONG lengthToReceive)
	{
		pEol->m_type = IO_TYPE::RECEIVE;
		DWORD flag = 0;
		WSABUF wsaBuffer;
		wsaBuffer.len = lengthToReceive;			// packet length is 1 byte.
		wsaBuffer.buf = &pEol->m_pBuffer[pEol->m_offset];
		int recvResult = WSARecv(pEol->m_socket, &wsaBuffer, 1, nullptr, &flag, pEol, nullptr);

		return recvResult;
	}

	int GBServer::IssueSend(GBExpandedOverlapped* pEol)
	{
		WSABUF bufToSend;
		DWORD sentBytes = 0;
		bufToSend.buf = pEol->m_pBuffer;
		bufToSend.len = pEol->m_offset;
		int sendResult = WSASend(pEol->m_socket, &bufToSend, 1, &sentBytes, 0, pEol, nullptr);

		return sendResult;
	}

	bool GBServer::Send(const GBExpandedOverlapped* pEol)
	{
		// 큐잉

		return true;
	}

	void GBServer::SendThreadFunction()
	{
		while (m_keepLooping)
		{
			GBExpandedOverlapped eol;
			if (m_sendQueue.pop(eol))
			{
				IssueSend(&eol);
			}
		}
	}
}