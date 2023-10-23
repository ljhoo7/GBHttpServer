#pragma once

#define FD_SETSIZE 2

#include "../Shared/GBShared.h"

#include "winsock2.h"
#include "MSWSock.h"

#include <string_view>

namespace GenericBoson
{
	class GBClient : public GBShared
	{
	public:
		virtual ~GBClient();
	protected:
		virtual int Connect(const std::string_view address, const int port);
	private:
		int InitializeWinSock();
		int CreateSocket();
		int ConnectInternal(const std::string_view address, const int port);
	private:
		WSADATA m_wsaData;
		SOCKET m_socket;
	};
}