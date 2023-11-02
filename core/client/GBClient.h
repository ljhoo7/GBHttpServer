#pragma once

#include "../Shared/GBShared.h"

#include "winsock2.h"

#include <string_view>

namespace GenericBoson
{
	class GBClient
	{
	public:
		virtual ~GBClient();
	protected:
		virtual int Connect(const std::string_view address, const int port);
	private:
		int InitializeWinSock();
		int CreateSocket();
		int ConnectInternal(const std::string_view address, const int port);
	protected:
		GBShared m_CoreShared;

		WSADATA m_wsaData;
		SOCKET m_socket;
	};
}