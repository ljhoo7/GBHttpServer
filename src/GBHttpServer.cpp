#include "../include/stdafx.h"

#include "../include/GBHttpServer.h"

namespace GenericBoson
{
	bool GBHttpServer::TraversePathTree(const std::vector<std::string>& pathTree, PathSegment*& pTargetPath)
	{
		for (auto& iPathSegment : pathTree)
		{
			// 원래 true == pTargetPath->m_subTreeMap.contains(iPathSegment) 였는데,
			// travis가 VS2017까지만 지원해서 아래와 같이 바꿈.
			if (pTargetPath->m_subTreeMap.end() == pTargetPath->m_subTreeMap.find(iPathSegment))
			{
				pTargetPath->m_subTreeMap.emplace(iPathSegment, std::make_unique<PathSegment>());
			}

			pTargetPath = (pTargetPath->m_subTreeMap[iPathSegment]).get();
		}

		if (nullptr != pTargetPath->m_pGetMethod)
		{
			// #ToDo
			// The action method Already Exists at the path.
			return false;
		}

		return true;
	}

	std::string GBHttpServer::GetWSALastErrorString()
	{
		char* s = NULL;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&s, 0, NULL);
		std::string errorString(s);
		LocalFree(s);

		return errorString;
	}

	std::string GBHttpServer::GetWSALastErrorString(int lastError)
	{
		char* s = NULL;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, lastError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&s, 0, NULL);
		std::string errorString(s);
		LocalFree(s);

		return errorString;
	}

	std::pair<bool, std::string> GBHttpServer::SetListeningSocket()
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
			m_threadPool.emplace_back(ThreadFunction);
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

	GBHttpServer::~GBHttpServer()
	{
		// winsock2 종료 처리
		closesocket(m_listeningSocket);
		WSACleanup();

		g_keepLooping = false;
	}

	int GBHttpServer::IssueRecv(GBExpandedOverlapped* pEol, ULONG lengthToReceive)
	{
		pEol->m_type = IO_TYPE::RECEIVE;
		DWORD flag = 0, receivedBytes = 0;
		WSABUF wsaBuffer;
		wsaBuffer.len = lengthToReceive;			// packet length is 1 byte.
		wsaBuffer.buf = &pEol->m_buffer[pEol->m_offset];
		int recvResult = WSARecv(pEol->m_socket, &wsaBuffer, 1, &flag, &receivedBytes, pEol, nullptr);

		return recvResult;
	}

	int GBHttpServer::IssueSend(GBExpandedOverlapped* pEol)
	{
		return -1;
	}

	void GBHttpServer::ThreadFunction()
	{
		DWORD receivedBytes;
		u_long completionKey;
		GBExpandedOverlapped* pEol = nullptr;

		while (true == g_keepLooping)
		{
			BOOL result = GetQueuedCompletionStatus(g_IOCP, &receivedBytes, (PULONG_PTR)&completionKey, (OVERLAPPED**)&pEol, INFINITE);

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
				bool parseResult = pEol->GatherAndParseLines(receivedBytes);
				pEol->m_offset += receivedBytes;

				bool gatheringNotFinished = false;
				bool gatheringFinishedButNothing = false;
				
				if (false == parseResult)
				{
					gatheringNotFinished = true;
				}
				else if (0 == pEol->m_offset)
				{
					gatheringFinishedButNothing = true;
				}

				// 개더링이 끝나지 않았거나, 끝났어도 받은게 전혀없다면, 더 받으려고 한다.
				if (true == gatheringNotFinished || true == gatheringFinishedButNothing)
				{
					int issueRecvResult = IssueRecv(pEol, BUFFER_SIZE - pEol->m_offset);
					int lastError = WSAGetLastError();

					if (SOCKET_ERROR == issueRecvResult && WSA_IO_PENDING != lastError)
					{
						// #ToDo
						// Issue receiving failed.
					}

					continue;
				}

				assert(0 < pEol->m_lines.size());

				// ExtendedOverlapped.GatherAndParseLines에서 빠져나왔다는 것은 최소 1줄은 읽었다는 것이다.
				GBHttpRequestLineReader requestLineReader(pEol->m_lines);
				
				GBRequestLineInformation requestLineInfo;
				bool succeeded = requestLineReader.ParseAndRead(&requestLineInfo);

				GBHttpHeaderInformation headerInfo;
				if(1 < pEol->m_lines.size())
				{
					// 헤더읽기
					GBHttpHeaderInformation info;
					GBHttpHeaderReader headerReader(pEol->m_lines);

					succeeded = headerReader.ParseAndRead(&info);
				}
				
#if defined(_DEBUG)
				// 통신 표시
				std::cout << pEol->m_buffer << '\n';
#endif
				{
					std::lock_guard<std::mutex> lock(g_mainCriticalsection);

					switch (requestLineInfo.m_version)
					{
					case HttpVersion::Http09:
					{
						g_pRouter = std::make_unique<GBHttpRouter<GBHttp09>>();
					}
					break;
					case HttpVersion::Http10:
					{
						g_pRouter = std::make_unique<GBHttpRouter<GBHttp10>>();
					}
					break;
					case HttpVersion::Http11:
					{
						g_pRouter = std::make_unique<GBHttpRouter<GBHttp11>>();
					}
					break;
					case HttpVersion::None:
					{
						// #ToDo 로깅으로 바꾸자
						//return { false, "An abnormal line exists in HTTP message.\n" };
					}
					break;
					default:
						assert(false);
					}

					//g_pRouter->m_methodList.emplace_back("GET", [](const std::string_view path)
					//{
					//	std::cout << "GET : path = " << path.data() << std::endl;
					//});
					//g_pRouter->m_methodList.emplace_back("PUT", [](const std::string_view path)
					//{
					//	std::cout << "PUT : path = " << path.data() << std::endl;
					//});
					//g_pRouter->m_methodList.emplace_back("POST", [](const std::string_view path)
					//{
					//	std::cout << "POST : path = " << path.data() << std::endl;
					//});

					bool routingResult = g_pRouter->Route(g_rootPath, requestLineInfo.m_targetPath, requestLineInfo.m_methodName);

					if (false == routingResult)
					{
						// #ToDo 로깅으로 바꾸자
						//return { false, "Routing failed." };
					}
				}

				pEol->m_offset = 0;
				int issueSendResult = IssueSend(pEol);
			}
			break;
			case IO_TYPE::SEND:
			{
				// 소켓 닫기
				closesocket(pEol->m_socket);
			}
			break;
			}
		}
	}

	std::pair<bool, std::string> GBHttpServer::Start()
	{
		bool result;
		std::string errorMsg;
		std::tie(result, errorMsg) = SetListeningSocket();

		m_sessions.resize(ISSUED_ACCEPTEX_COUNT);

		// AcceptEx 이슈
		for(int k = 0; k < ISSUED_ACCEPTEX_COUNT; ++k)
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

	bool GBHttpServer::GET(const std::string_view targetPath, const std::function<void(int)>& func)
	{
		std::lock_guard<std::mutex> lock(g_mainCriticalsection);

		std::vector<std::string> pathSegmentArray;
		bool parseResult = ParseUrlString(targetPath, pathSegmentArray);

		if (false == parseResult)
		{
			// #ToDo
			// targetPaht does not start with '/'.
			return false;
		}

		PathSegment* pTargetPath = &g_rootPath;
		bool traverseResult = TraversePathTree(pathSegmentArray, pTargetPath);

		if (false == traverseResult)
		{
			// #ToDo
			// The action method Already Exists at the path.
			return false;
		}

		pTargetPath->m_pGetMethod = std::make_unique<GBMethodGET>();
		pTargetPath->m_pGetMethod->m_method = func;

		return true;
	}

	bool GBHttpServer::HEAD(const std::string_view targetPath, const std::function<void(int)>& func)
	{
		std::lock_guard<std::mutex> lock(g_mainCriticalsection);

		std::vector<std::string> pathSegmentArray;
		bool parseResult = ParseUrlString(targetPath, pathSegmentArray);

		if (false == parseResult)
		{
			// #ToDo
			// targetPaht does not start with '/'.
			return false;
		}

		PathSegment* pTargetPath = &g_rootPath;
		bool traverseResult = TraversePathTree(pathSegmentArray, pTargetPath);

		if (false == traverseResult)
		{
			// #ToDo
			// The action method Already Exists at the path.
			return false;
		}

		pTargetPath->m_pHeadMethod = std::make_unique<GBMethodHEAD>();
		pTargetPath->m_pHeadMethod->m_method = func;

		return true;
	}

	bool GBHttpServer::POST(const std::string_view targetPath, const std::function<void(int)>& func)
	{
		std::lock_guard<std::mutex> lock(g_mainCriticalsection);

		std::vector<std::string> pathSegmentArray;
		bool parseResult = ParseUrlString(targetPath, pathSegmentArray);

		if (false == parseResult)
		{
			// #ToDo
			// targetPaht does not start with '/'.
			return false;
		}

		PathSegment* pTargetPath = &g_rootPath;
		bool traverseResult = TraversePathTree(pathSegmentArray, pTargetPath);

		if (false == traverseResult)
		{
			// #ToDo
			// The action method Already Exists at the path.
			return false;
		}

		pTargetPath->m_pPostMethod = std::make_unique<GBMethodPOST>();
		pTargetPath->m_pPostMethod->m_method = func;

		return true;
	}

	std::mutex GBHttpServer::g_mainCriticalsection;

	PathSegment GBHttpServer::g_rootPath;
	std::unique_ptr<GBHttpRouterBase> GBHttpServer::g_pRouter;

	HANDLE GBHttpServer::g_IOCP = INVALID_HANDLE_VALUE;
	volatile bool GBHttpServer::g_keepLooping = true;
}