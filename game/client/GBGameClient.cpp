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
			m_pShared->ErrorLog("");
		}
		else if (retval)
		{
			// Data is available now.
			for (int k = 0; k < FD_SETSIZE; ++k)
			{
				if (FD_ISSET(k, &m_sockets))
				{
					int readBytes = recv(k, m_inputData.m_buffer, BUFFER_SIZE, 0);
					bool ret = m_pShared->OnReceived(m_inputData, readBytes);
				}
			}
		}

		return m_keepLooping.load();
	}
	int GBGameClient::Connect(const std::string_view address, const int port)
	{
		FD_ZERO(&m_sockets);
		FD_SET(0, &m_sockets);

		static_assert(FD_SETSIZE == 2);

		return __super::Connect(address, port);
	}
}