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
        int retval = select(m_socket + 1, &m_reads, NULL, NULL, &m_peekInterval);

		if (retval == -1)
		{
			m_GameShared.ErrorLog("");
		}
		else if (retval)
		{
			// Data is available now.
			for (int k = 0; k < m_socket + 1; ++k)
			{
				if (FD_ISSET(k, &m_reads))
				{
					int readBytes = recv(k, m_inputData.m_buffer, BUFFER_SIZE, 0);
					bool ret = m_GameShared.OnReceived(m_inputData, readBytes);
				}
			}
		}

		//int readBytes = recv(m_socket, m_inputData.m_buffer, BUFFER_SIZE, 0);

		return m_keepLooping.load();
	}
	int GBGameClient::Connect(const std::string_view address, const int port)
	{
		FD_ZERO(&m_reads);
		FD_SET(0, &m_reads);

		return __super::Connect(address, port);
	}
}