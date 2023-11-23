#pragma once

#define NOMINMAX

#include "../shared/GameShared.h"
#include "../../core/client/Client.h"
#include "../../flatbuffers/include/flatbuffers/flatbuffers.h"

#include <string>

namespace GenericBoson
{
	class GBGameClient : public GBClient
	{
	public:
		virtual ~GBGameClient();

		template<typename FLATBUFFER_TABLE>
		bool AddStub(const int messageID, void(*Stub)(const FLATBUFFER_TABLE& table))
		{
			return m_GameShared.AddStubInternal(messageID, Stub);
		}

		bool GetKeepLooping();

		virtual int Connect(const std::string_view address, const int port) override;
	private:
		GBGameShared m_GameShared;

		std::atomic_bool	m_keepLooping = true;

		VectoredIO			m_inputData;
		VectoredIO			m_outputData;

		fd_set				m_reads, m_writes;
	};
}