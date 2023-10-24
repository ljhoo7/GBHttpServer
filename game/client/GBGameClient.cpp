#include "stdafx.h"

#include "GBGameClient.h"

namespace GenericBoson
{
	GBGameClient::~GBGameClient()
	{
		m_keepLooping.store(false);
	}

	bool GBGameClient::GetKeepLooping()
	{
        int retval = select(1, &m_sockets, NULL, NULL, &m_peekInterval);

		if (retval == -1)
		{
			ErrorLog("");
		}
		else if (retval)
		{
			// Data is available now.
			switch (retval)
			{
			case 1:
				break;
			case 2:
				break;
			}
		}

		return m_keepLooping.load();
	}
	int GBGameClient::Connect(const std::string_view address, const int port)
	{
		FD_ZERO(&m_sockets);
		FD_SET(0, &m_sockets);

		return __super::Connect(address, port);
	}
}