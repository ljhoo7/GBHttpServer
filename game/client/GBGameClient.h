#pragma once

#define NOMINMAX

#include "../shared/GBGameShared.h"
#include "../../core/client/GBClient.h"
#include "../../flatbuffers/include/flatbuffers/flatbuffers.h"

#include <string>

namespace GenericBoson
{
	class GBGameClient : public GBClient
	{
	public:
		GBGameClient() : m_pShared{ std::make_unique<GBGameShared>()}, GBClient()
		{}
		virtual ~GBGameClient();

		template<typename FLATBUFFER_TABLE>
		bool AddStub(const int messageID, void(*Stub)(const FLATBUFFER_TABLE& table))
		{
			return m_pShared->AddStubInternal(messageID, Stub);
		}

		bool GetKeepLooping();

		virtual int Connect(const std::string_view address, const int port) override;
	private:
		std::atomic_bool	m_keepLooping = true;

		VectoredIO			m_inputData;
		VectoredIO			m_outputData;

		fd_set				m_sockets;
		timeval				m_peekInterval
		{
			.tv_sec = 0,
			.tv_usec = 500
		};
	};
}