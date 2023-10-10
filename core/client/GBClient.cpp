#include "stdafx.h"

#include "GBClient.h"
#include "../server/GBUtil.h"

#include <string_view>

namespace GenericBoson
{
	GBClient::~GBClient()
	{
		const auto result = closesocket(m_socket);
		if (result == SOCKET_ERROR)
		{
			// #ToDo logging error string
		}

		WSACleanup();
	}

	int GBClient::InitializeWinSock()
	{
		return WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	}

	int GBClient::CreateSocket()
	{
		const auto m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET) {
			int errorCode = WSAGetLastError();
			// # ToDo error logging
			WSACleanup();

			return errorCode;
		}

		return NO_ERROR;
	}

	int GBClient::ConnectInternal(const std::string_view address, const int port)
	{
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(address.data());
		addr.sin_port = htons(port);

		if (connect(m_socket, (SOCKADDR*)&addr, sizeof(addr))
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
		int result = NO_ERROR;
		result = InitializeWinSock();
		if (result)
		{
			return result;
		}

		result = CreateSocket();
		if (result)
		{
			// #ToDo error logging
			return result;
		}

		result = ConnectInternal(address, port);
		if (result)
		{
			// #ToDo error logging
		}

		return result;
	}
}