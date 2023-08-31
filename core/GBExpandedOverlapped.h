#pragma once

#include "boost/lockfree/queue.hpp"
#include "winsock2.h"

#include <queue>

namespace GenericBoson
{
	typedef short BUFFER_TYPE;
	constexpr BUFFER_TYPE BUFFER_SIZE = 8 * 1024;

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

		enum STATE : char
		{
			ID = 0,
			LENGTH = 1,
			PAYLOAD = 2,
		};

		STATE m_scatterState = STATE::ID;
		STATE m_gatherState = STATE::ID;

		BUFFER_TYPE m_length = 0;
		BUFFER_TYPE m_recvOffset = 0, m_sendOffset = 0;
	};
}