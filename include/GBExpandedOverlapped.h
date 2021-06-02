#pragma once

namespace GenericBoson
{
	const int BUFFER_SIZE = 4096;

	enum class IO_TYPE : int64_t
	{
		ACCEPT = 1,
		RECEIVE,
		SEND,
	};

	struct GBExpandedOverlapped : public WSAOVERLAPPED
	{
		SOCKET m_socket = INVALID_SOCKET;
		IO_TYPE m_type = IO_TYPE::ACCEPT;
		uint32_t m_leftBytesToReceive = 0;

		// #ToDo
		// This must be exchanged with a circular lock-free queue.
		char m_buffer[BUFFER_SIZE] = { 0, };
		int m_offset = 0;

		/*
		lines parsed by GatherAndParseLines function.
		*/
		std::vector<std::string> m_lines;
		
		/*
		GatherAndParseLines the HTTP message with gathering.

		\return Was It succeeded?
		*/
		bool GatherAndParseLines();
	};
}