#pragma once

#define NOMINMAX

#include "../../core/client/GBClient.h"
#include "../../flatbuffers/include/flatbuffers/flatbuffers.h"

#include <string>

namespace GenericBoson
{
	class GBGameClient : public GBClient
	{
	public:
		virtual ~GBGameClient();

		template<typename FLATBUFFER_TABLE>
		void AddStub(const int messageID, void(*stub)(const FLATBUFFER_TABLE& player))
		{

		}

		bool GetKeepLooping();

		virtual int Connect(const std::string_view address, const int port) override;
	private:

		std::atomic_bool m_keepLooping = true;
	};
}