#include "../include/stdafx.h"

#include "../include/GBResponse.h"

namespace GenericBoson
{
	bool SendResponse(const SOCKET& acceptedSocket, const GBStringView path)
	{
		char html[1024] = { 0, };

		// HTTP
		const char* header =
			"HTTP/1.0 200 OK\n"
			"Content-type: text/html\n";

		int sendResult = send(acceptedSocket, header, strlen(header), 0);
		if (sendResult < 1)
		{
			std::cout << "send response header failed : " << WSAGetLastError() << '\n';

			return false;
		}

		// 라우팅 
		if (_T("/page1") == path)
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
		else if (_T("/page2") == path)
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

		// 응답（HTML을 보낸다
		sendResult = send(acceptedSocket, html, strlen(html), 0);
		if (sendResult < 1)
		{
			std::cout << "send response body failed : " << WSAGetLastError() << '\n';

			return false;
		};

		return true;
	}
}