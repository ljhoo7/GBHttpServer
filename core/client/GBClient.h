#pragma once

#include "winsock2.h"
#include "MSWSock.h"

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
	private:
		WSADATA m_wsaData;
		SOCKET m_socket;
	};
}