#pragma once

#define NOMINMAX

#include "../shared/GBGameShared.h"
#include "../../core/client/GBClient.h"
#include "../../flatbuffers/include/flatbuffers/flatbuffers.h"

#include <string>

namespace GenericBoson
{
	class GBGameClient : public GBClient, public GBGameShared
	{
	public:
		virtual ~GBGameClient();

		bool GetKeepLooping();

		virtual int Connect(const std::string_view address, const int port) override;
	private:

		std::atomic_bool m_keepLooping = true;
	};
}