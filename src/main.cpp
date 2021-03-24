#include "../include/stdafx.h"

#include "../include/GBMethod.h"
#include "../include/GBRouter.h"

#pragma comment(lib, "ws2_32.lib")
 
WSADATA     wsaData;
SOCKET      sock0;
SOCKET      sockw;
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
	char html[1024];
 
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
	sock0 = socket(AF_INET, SOCK_STREAM, 0);
	if (sock0 == INVALID_SOCKET)
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
	if (bind(sock0, (struct sockaddr *)&addr, sizeof(addr)) != 0)
	{
		printf("bind : %d\n", WSAGetLastError());
		getch();
		return -1;
	}
 
	// TCP 클라이언트로부터의 접속 요구를 기다리면서 대기한다
	if (listen(sock0, 5) != 0)
	{
		printf("listen : %d\n", WSAGetLastError());
		getch();
		return -1;
	}
 
	// 서버 시작
	while (1)
	{
		len = sizeof(client);
		sockw = accept(sock0, (struct sockaddr *)&client, &len);
		if (sockw == INVALID_SOCKET)
		{
			printf("accept : %d\n", WSAGetLastError());
			break;
		}
 
		// 버퍼 초기화
		memset(html, 0, 1024);
 
		// 접속
		recv_len = recvfrom(sockw, buf, 1024, 0, (struct sockaddr *)&client, &sockaddr_in_size);

		std::string_view bufString(buf);
		buf[recv_len - 1] = 0;
		if ('\0' == buf[0])
		{
			strcpy(buf, NULL);
		}

		// 통신 표시
		std::cout << buf << '\n';

		GenericBoson::GBRouter router;
		router.m_methodList.emplace_back("GET", []()
		{
			std::cout << "test1" << std::endl;
		});
		router.m_methodList.emplace_back("PUT", []()
		{
			std::cout << "test2" << std::endl;
		});
		router.m_methodList.emplace_back("POST", []()
		{
			std::cout << "test3" << std::endl;
		});
 
		// method
		std::string_view path = router.Route(bufString);

		std::cout << "request: " << path.data() << '\n';
 
		// HTTP
		char *header =  
			"HTTP/1.0 200 OK\n"
			"Content-type: text/html\n"
			"\n";
 
		send(sockw, header, strlen(header), 0);
 
		// 라우팅 
		if ("/page1" == path)
		{
			strcpy(html,
			"<!DOCTYPE html>\n"
			"<html lang = \"ja\">\n"
			"<head>\n"
			"<meta charset = \"utf-8\">\n"
			"</head>\n"
			"<body>\n"
			"<h1>Page1</h1>\n"
			"<a href=\"/page2\">->page2</a>\n"
			"</body>"
			"</html>");
		}
		else if ("/page2" == path)
		{
			strcpy(html,
			"<!DOCTYPE html>\n"
			"<html lang = \"ja\">\n"
			"<head>\n"
			"<meta charset = \"utf-8\">\n"
			"</head>\n"
			"<body>\n"
			"<h1>Page2</h1>\n"
			"<a href=\"/page1\">->page1</a>\n"
			"</body>"
			"</html>");
		}
		else
		{
			strcpy(html,
			"<!DOCTYPE html>\n"
			"<html lang = \"ja\">\n"
			"<head>\n"
			"<meta charset = \"utf-8\">\n"
			"</head>\n"
			"<body>\n"
			"<h1>404- Not Found</h1>\n"
			"</body>"
			"</html>");
		}
 
		// 응답（HTML을 보낸다）
		if (send(sockw, html, strlen(html), 0) < 1)
		{
			printf("send : %d\n", WSAGetLastError());
			break;
		}
 
		// 소켓 닫기
		closesocket(sockw);
	}
 
	// winsock2 종료 처리
	closesocket(sock0);
	WSACleanup();
	return 0;
}