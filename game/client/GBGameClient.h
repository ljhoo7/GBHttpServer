#pragma once

#include "../../core/client/GBClient.h"

#include <string>

namespace GenericBoson
{
	class GBGameClient : public GBClient
	{
	public:
		virtual ~GBGameClient() = default;

		virtual int Connect(const std::string_view address, const int port) override;
	private:
	};
}