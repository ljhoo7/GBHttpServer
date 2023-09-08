#pragma once

#include "boost/lockfree/queue.hpp"
#include "winsock2.h"

#include <queue>

namespace GenericBoson
{
	typedef uint64_t BUFFER_SIZE_TYPE;
	constexpr BUFFER_SIZE_TYPE BUFFER_SIZE = 8 * 1024;

	enum class IO_TYPE : int32_t
	{
		ACCEPT = 1,
		RECEIVE,
		SEND,
	};

	class VectoredIO
	{
	public:
		enum STATE : char
		{
			ID = 0,
			LENGTH,
			PAYLOAD,
			MAX_STATE
		};
	public:
		void AdvanceState()
		{
			m_state = STATE{ (char{ m_state } + 1) % STATE::MAX_STATE };
		}

		STATE GetState() const
		{
			return m_state;
		}

	public:
		char* m_pBuffer = nullptr;
		BUFFER_SIZE_TYPE m_length = 0, m_offset = 0;

	private:
		STATE m_state = STATE::ID;
	};

	struct GBExpandedOverlapped : public WSAOVERLAPPED
	{
		SOCKET m_socket = INVALID_SOCKET;
		IO_TYPE m_type = IO_TYPE::ACCEPT;

		VectoredIO m_gatherInput, m_scatterOutput;
	};
}