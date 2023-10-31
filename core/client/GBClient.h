#pragma once

#define FD_SETSIZE 2

#include "../Shared/GBShared.h"

#include "winsock2.h"

#include <string_view>

namespace GenericBoson
{
	class GBClient
	{
	public:
		GBClient()
		{
			if (!m_pShared)
			{
				m_pShared = std::make_unique<GBShared>();
			}
		}
		virtual ~GBClient();
	protected:
		virtual int Connect(const std::string_view address, const int port);
	private:
		int InitializeWinSock();
		int CreateSocket();
		int ConnectInternal(const std::string_view address, const int port);
	public:
		std::unique_ptr<GBShared> m_pShared;
	private:
		WSADATA m_wsaData;
		SOCKET m_socket;
	};
}