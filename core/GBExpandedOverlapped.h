#pragma once

#include "boost/lockfree/queue.hpp"
#include "winsock2.h"

#include <queue>

namespace GenericBoson
{
	constexpr short BUFFER_SIZE = 8 * 1024;

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

		std::decay_t<decltype(BUFFER_SIZE)> m_length = 0;
		std::decay_t<decltype(BUFFER_SIZE)> m_recvOffset = 0, m_sendOffset = 0;
	};
}