#include "stdafx.h"

#include "GBClient.h"

#include <string_view>

namespace GenericBoson
{
	int InitializeWinSock()
	{
		return WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	}

	int CreateSocket()
	{
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET) {
			int errorCode = WSAGetLastError());
			WSACleanup();
			return errorCode;
		}

		return NO_ERROR;
	}

	int Connect(const std::string_view address, const int port)
	{
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(address.data());
		addr.sin_port = htons(port);

		if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService))
			== SOCKET_ERROR)
		{
			int errorCode = WSAGetLastError();
			if (closesocket(m_socket) == SOCKET_ERROR)
			{
				// #ToDo error logging
			}

			WSACleanup();
			return errorCode;
		}

		return NO_ERROR;
	}

	int GBClient::Connect(const std::string_view address, const int port)
	{
		int errorCode = NO_ERROR;
		errorCode = InitializeWinSock();
		if (errorCode)
		{
			// #ToDo error logging
			return errorCode;
		}

		errorCode = CreateSocket();
		if (errorCode)
		{
			// #ToDo error logging
			return errorCode;
		}

		errorCode = Connect(address, port);
		if (errorCode)
		{
			// #ToDo error logging
			return errorCode;
		}

		return errorCode;
	}
}