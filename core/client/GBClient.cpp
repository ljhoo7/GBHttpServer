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
			auto errorString = GetWSALastErrorString();
			// #ToDo logging error string
		}

		WSACleanup();
	}

	WSADATA InitializeWinSock()
	{
		WSADATA wsaData;
		const auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result)
		{
			// #ToDo error logging
		}

		return wsaData;
	}

	SOCKET CreateSocket()
	{
		const auto result = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (result == INVALID_SOCKET) {
			int errorCode = WSAGetLastError();
			// # ToDo error logging
			WSACleanup();
		}

		return result;
	}

	int ConnectInternal(SOCKET socket, const std::string_view address, const int port)
	{
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(address.data());
		addr.sin_port = htons(port);

		if (connect(socket, (SOCKADDR*)&addr, sizeof(addr))
			== SOCKET_ERROR)
		{
			int errorCode = WSAGetLastError();
			if (closesocket(socket) == SOCKET_ERROR)
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

		errorCode = ConnectInternal(m_socket, address, port);
		if (errorCode)
		{
			// #ToDo error logging
			return errorCode;
		}

		return errorCode;
	}
}