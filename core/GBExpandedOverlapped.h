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

		// #ToDo
		// This must be exchanged with a circular lock-free queue.
		char m_recvBuffer[BUFFER_SIZE] = { 0, };
		char m_sendBuffer[BUFFER_SIZE] = { 0, };

		int m_test = 0;

		DWORD m_recvOffset = 0, m_sendOffset = 0;
	};
}