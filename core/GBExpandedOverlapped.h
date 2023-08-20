#pragma once

#include "winsock2.h"

#include <queue>

namespace GenericBoson
{
	const int BUFFER_SIZE = 4096;

	enum class IO_TYPE : int32_t
	{
		ACCEPT = 1,
		RECEIVE,
		SEND,
	};

	struct GBExpandedOverlapped : public WSAOVERLAPPED
	{
		SOCKET m_socket = INVALID_SOCKET;
		IO_TYPE m_type = IO_TYPE::ACCEPT;

		char* m_pRecvBuffer = nullptr;
		char* m_pSendBuffer = nullptr;

		DWORD m_recvOffset = 0, m_sendOffset = 0;
	};
}