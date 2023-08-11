#pragma once

#include <queue>

#include "GBHttpVersionTypes.h"

namespace GenericBoson
{
	const int BUFFER_SIZE = 4096;

	enum class IO_TYPE : uint64_t
	{
		ACCEPT = 1,
		RECEIVE,
		SEND,
	};

	struct GBExpandedOverlapped : public WSAOVERLAPPED
	{
		HttpVersion m_httpVersion;

		SOCKET m_socket = INVALID_SOCKET;
		IO_TYPE m_type = IO_TYPE::ACCEPT;

		// #ToDo
		// This must be exchanged with a circular lock-free queue.
		char m_recvBuffer[BUFFER_SIZE] = { 0, };
		char m_sendBuffer[BUFFER_SIZE] = { 0, };

		int m_test = 0;

		DWORD m_recvOffset = 0, m_sendOffset = 0;

		/*
		lines parsed by GatherAndParseLines function.
		*/
		std::queue<std::string_view> m_lines;
		
		/*
		GatherAndParseLines the HTTP message with gathering.

		\return Was It succeeded?
		*/
		bool GatherAndParseLines(DWORD receivedBytes);
	};
}