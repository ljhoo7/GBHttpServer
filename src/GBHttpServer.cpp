#include "../include/stdafx.h"

#include "../include/GBHttpServer.h"

namespace GenericBoson
{
	bool GBHttpServer::TraversePathTree(const std::vector<std::string>& pathTree, PathSegment*& pTargetPath)
	{
		for (auto& iPathSegment : pathTree)
		{
			if (false == pTargetPath->m_subTreeMap.contains(iPathSegment))
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

	std::pair<bool, std::string> GBHttpServer::SetListeningSocket()
	{
		// WinSock 2.2 초기화
		if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &m_wsaData))
		{
			return { false, "WSAStartup failed\n" };
		}

		// IOCP 커널 오브젝트 만들기.
		m_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (u_long)0, 0);
		if (NULL == m_IOCP)
		{
			return { false, "" };
		}

		// 소켓 만들기
		m_listeningSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
		if (INVALID_SOCKET == m_listeningSocket)
		{
			std::cout << "socket : " << WSAGetLastError() << '\n';
			return;
		}

		// Associate the listening socket with the IOCP.
		HANDLE ret1 = CreateIoCompletionPort((HANDLE)m_listeningSocket, m_IOCP, (u_long)0, 0);

		if (NULL == ret1)
		{
			return { false, WSAGetLastError() };
		}

		// 소켓 설정
		m_addr.sin_family = AF_INET;
		m_addr.sin_port = htons(m_port);
		m_addr.sin_addr.S_un.S_addr = INADDR_ANY;

		// 소켓 바인드
		if (bind(m_listeningSocket, (struct sockaddr*)&m_addr, sizeof(m_addr)) != 0)
		{
			std::cout << "bind : " << WSAGetLastError() << '\n';
			return;
		}

		// TCP 클라이언트로부터의 접속 요구를 기다리면서 대기한다
		if (listen(m_listeningSocket, 5) != 0)
		{
			std::cout << "listen : " << WSAGetLastError() << '\n';
			return;
		}
	}

	GBHttpServer::~GBHttpServer()
	{
		// winsock2 종료 처리
		closesocket(m_listeningSocket);
		WSACleanup();
	}

	bool GBHttpServer::Start()
	{
		bool result;
		std::string errorMsg;
		std::tie(result, errorMsg) = SetListeningSocket();

		// 서버 시작
		while (1)
		{
			SOCKET acceptedSocket = accept(m_listeningSocket, (sockaddr*)&m_client, &m_addrSize);
			if (acceptedSocket == INVALID_SOCKET)
			{
				std::cout << "accept : " << WSAGetLastError() << '\n';
				return false;
			}

			// 접속
			int recved = recvfrom(acceptedSocket, m_buffer, 1024, 0, (sockaddr*)&m_client, &m_addrSize);

			std::string_view bufString(m_buffer);

#if defined(_DEBUG)
			// 통신 표시
			GBCout << m_buffer << '\n';
#endif
			std::string targetPath, methodName;
			GenericBoson::GBHttpRequestLineReader requestLineReader;
			HttpVersion version = requestLineReader.Read(m_buffer, targetPath, methodName);

			switch (version)
			{
			case HttpVersion::Http09:
			{
				m_pRouter = std::make_unique<GBHttpRouter<GBHttp09>>(acceptedSocket);
			}
			break;
			case HttpVersion::Http10:
			{
				m_pRouter = std::make_unique<GBHttpRouter<GBHttp10>>(acceptedSocket);
			}
			break;
			case HttpVersion::Http11:
			{
				m_pRouter = std::make_unique<GBHttpRouter<GBHttp11>>(acceptedSocket);
			}
			break;
			case HttpVersion::None:
			{
				std::cout << "An abnormal line exists in HTTP message.\n";
				return false;
			}
			break;
			default:
				assert(false);
			}

			//m_pRouter->m_methodList.emplace_back("GET", [](const std::string_view path)
			//{
			//	std::cout << "GET : path = " << path.data() << std::endl;
			//});
			//m_pRouter->m_methodList.emplace_back("PUT", [](const std::string_view path)
			//{
			//	std::cout << "PUT : path = " << path.data() << std::endl;
			//});
			//m_pRouter->m_methodList.emplace_back("POST", [](const std::string_view path)
			//{
			//	std::cout << "POST : path = " << path.data() << std::endl;
			//});

			bool routingResult = m_pRouter->Route(m_rootPath, targetPath, methodName);

			if (false == routingResult)
			{
				std::cout << "Routing failed." << '\n';
				return false;
			}

			// 소켓 닫기
			closesocket(acceptedSocket);
		}

		return true;
	}

	bool GBHttpServer::GET(const std::string_view targetPath, const std::function<void(int)>& func)
	{
		std::vector<std::string> pathSegmentArray;
		bool parseResult = ParseUrlString(targetPath, pathSegmentArray);

		if (false == parseResult)
		{
			// #ToDo
			// targetPaht does not start with '/'.
			return false;
		}

		PathSegment* pTargetPath = &m_rootPath;
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
		std::vector<std::string> pathSegmentArray;
		bool parseResult = ParseUrlString(targetPath, pathSegmentArray);

		if (false == parseResult)
		{
			// #ToDo
			// targetPaht does not start with '/'.
			return false;
		}

		PathSegment* pTargetPath = &m_rootPath;
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
		std::vector<std::string> pathSegmentArray;
		bool parseResult = ParseUrlString(targetPath, pathSegmentArray);

		if (false == parseResult)
		{
			// #ToDo
			// targetPaht does not start with '/'.
			return false;
		}

		PathSegment* pTargetPath = &m_rootPath;
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
}