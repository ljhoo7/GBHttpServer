#include "../include/stdafx.h"

#include "../include/GBHttpServer.h"
#include "../include/GBHttpRequestLineReader.h"
#include "../include/GBHttpRouter.h"

namespace GenericBoson
{
	GBHttpServer::GBHttpServer(uint16_t port)
	{
		// winsock2 초기화
		if (WSAStartup(MAKEWORD(2, 0), &m_wsaData))
		{
			std::cout << "winsock startup failed\n";
			return;
		}

		// 소켓 만들기
		 m_listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_listeningSocket == INVALID_SOCKET)
		{
			std::cout << "socket : " << WSAGetLastError() << '\n';
			return;
		}

		// 소켓 설정
		m_addr.sin_family = AF_INET;
		m_addr.sin_port = htons(port);
		m_addr.sin_addr.S_un.S_addr = INADDR_ANY;

		// 소켓 바인드
		if (bind(m_listeningSocket, (struct sockaddr *)&m_addr, sizeof(m_addr)) != 0)
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
			std::string targetPath;
			GenericBoson::GBHttpRequestLineReader requestLineReader;
			HttpVersion version = requestLineReader.Read(m_buffer, targetPath);

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

			bool routingResult = m_pRouter->Route(m_methodTree);

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
}