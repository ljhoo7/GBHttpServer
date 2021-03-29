#include "../include/stdafx.h"

#include "../include/GBMethod.h"
#include "../include/GBRouter.h"

#pragma comment(lib, "ws2_32.lib")
 
WSADATA     wsaData;
struct      sockaddr_in addr;
struct      sockaddr_in client;
#define PORT_NUM 8000

// IP 주소 얻기
int getAddrHost(void)
{
	int i;
	HOSTENT *lpHost;       //  호스트 정보를 저장하는 구조체
	IN_ADDR inaddr;       // IP 주소를 저장하는 구조체
	char szBuf[256], szIP[16];  // 호스트 이름/IP 주소를 저장하는 배열
 
	// 에러 처리
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		printf("WSAStartup Error\n");
		return -1;
	}
 
	// 로컬 머신의 호스트 이름을 얻는다
	gethostname(szBuf, (int)sizeof(szBuf));
 
	// 호스트 정보 얻기
	lpHost = gethostbyname(szBuf);
	// IP 주소 얻기
	for (i = 0; lpHost->h_addr_list[i]; i++)
	{
		memcpy(&inaddr, lpHost->h_addr_list[i], 4);
	}

	strcpy(szIP, inet_ntoa(inaddr));
	printf("build server: http://%s:%d\n", szIP,PORT_NUM);  
 
	WSACleanup();
 
	return 0;
}

int main()
{
	int len;
	int n;
	int sockaddr_in_size = sizeof(struct sockaddr_in);
	int recv_len = 0;
	char buf[1024];
 
	//IP 어드레스 표시
	if (getAddrHost() != 0) {
		printf("get IP address failed");
		getch();
		return -1;
	}
 
	// winsock2 초기화
	if (WSAStartup(MAKEWORD(2, 0), &wsaData))
	{
		puts("reset winsock failed");
		getch();
		return -1;
	}
 
	// 소켓 만들기
	SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listeningSocket == INVALID_SOCKET)
	{
		printf("socket : %d\n", WSAGetLastError());
		getch();
		return -1;
	}
 
	// 소켓 설정
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NUM);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
 
	// 소켓 바인드
	if (bind(listeningSocket, (struct sockaddr *)&addr, sizeof(addr)) != 0)
	{
		printf("bind : %d\n", WSAGetLastError());
		getch();
		return -1;
	}
 
	// TCP 클라이언트로부터의 접속 요구를 기다리면서 대기한다
	if (listen(listeningSocket, 5) != 0)
	{
		printf("listen : %d\n", WSAGetLastError());
		getch();
		return -1;
	}
 
	// 서버 시작
	while (1)
	{
		len = sizeof(client);
		SOCKET acceptedSocket = accept(listeningSocket, (struct sockaddr *)&client, &len);
		if (acceptedSocket == INVALID_SOCKET)
		{
			printf("accept : %d\n", WSAGetLastError());
			break;
		}
 
		// 접속
		recv_len = recvfrom(acceptedSocket, buf, 1024, 0, (struct sockaddr *)&client, &sockaddr_in_size);

		std::string_view bufString(buf);
		buf[recv_len - 1] = 0;
		if ('\0' == buf[0])
		{
			strcpy(buf, NULL);
		}

		// 통신 표시
		std::cout << buf << '\n';

		GenericBoson::GBRouter router(acceptedSocket);
		router.m_methodList.emplace_back("GET", [](const std::string_view path)
		{
			std::cout << "GET : path = " << path.data() << std::endl;
		});
		router.m_methodList.emplace_back("PUT", [](const std::string_view path)
		{
			std::cout << "PUT : path = " << path.data() << std::endl;
		});
		router.m_methodList.emplace_back("POST", [](const std::string_view path)
		{
			std::cout << "POST : path = " << path.data() << std::endl;
		});
 
		// method
		bool routingResult = router.Route(bufString);

		if (false == routingResult)
		{
			std::cout << "Routing failed." << std::endl;
		}

		// 소켓 닫기
		closesocket(acceptedSocket);
	}
 
	// winsock2 종료 처리
	closesocket(listeningSocket);
	WSACleanup();
	return 0;
}