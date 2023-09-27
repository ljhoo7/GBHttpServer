#include "stdafx.h"

#include "GBGameClient.h"

namespace GenericBoson
{
	bool GBGameClient::GetKeepLooping()
	{
		return false;
	}
	int GBGameClient::Connect(const std::string_view address, const int port)
	{
		return __super::Connect(address, port);
	}
}