#pragma once

#include "boost/lockfree/queue.hpp"
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

	class GBExpandedOverlapped : public WSAOVERLAPPED
	{
	public:
		void SetIndex(const int index);
		int GetIndex() const;
		bool IsForSend() const;
	public:
		SOCKET m_socket = INVALID_SOCKET;
		IO_TYPE m_type = IO_TYPE::ACCEPT;

		char* m_pRecvBuffer = nullptr;
		char* m_pSendBuffer = nullptr;

		DWORD m_recvOffset = 0, m_sendOffset = 0;

	private:
		int m_index = 0;
	};
}