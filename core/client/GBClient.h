#pragma once

#include "winsock2.h"
#include "MSWSock.h"

namespace GenericBoson
{
	class GBClient
	{
	public:

	protected:
		virtual int Connect(const std::string_view address, const int port);
	private:
		WSADATA m_wsaData;
		SOCKET m_socket;
	};
}