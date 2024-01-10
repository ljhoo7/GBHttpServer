#pragma once

#include "../shared/Shared.h"

#include "winsock2.h"

#include <queue>

namespace GenericBoson
{
	struct ExpandedOverlapped : public WSAOVERLAPPED
	{
		SOCKET m_socket = INVALID_SOCKET;
		IO_TYPE m_type = IO_TYPE::ACCEPT;

		VectoredIO m_inputData, m_outputData;
	};
}