#pragma once

#include "../Shared/Shared.h"

#include "winsock2.h"

#include <string_view>

namespace GenericBoson
{
	class GBClient
	{
	public:
		virtual ~GBClient();

		template<typename FLATBUFFER_TABLE>
		bool AddStub(const int messageID, void(*Stub)(const FLATBUFFER_TABLE& table))
		{
			return m_CoreShared.AddStubInternal(messageID, Stub);
		}

		bool GetKeepLooping();

		int Connect(const std::string_view address, const int port);
	private:
		int InitializeWinSock();
		int CreateSocket();
		int ConnectInternal(const std::string_view address, const int port);
	private:
		CoreShared m_CoreShared;

		WSADATA m_wsaData;
		SOCKET m_socket;

		std::atomic_bool	m_keepLooping = true;

		VectoredIO			m_inputData;
		VectoredIO			m_outputData;

		fd_set				m_reads, m_writes;
	};
}