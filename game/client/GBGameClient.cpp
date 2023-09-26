#include "stdafx.h"

#include "GBGameClient.h"

namespace GenericBoson
{
	int GBGameClient::Connect(const std::string_view address, const int port)
	{
		return __super::Connect(address, port);
	}
}