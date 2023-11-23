#include "stdafx.h"

#include "GameClient.h"
#include <iostream>

namespace GenericBoson
{
	GBGameClient::~GBGameClient()
	{
		m_keepLooping.store(false);
	}

	bool GBGameClient::GetKeepLooping()
	{
		timeval peekInterval;

		peekInterval.tv_sec = 0;
		peekInterval.tv_usec = 0;

		fd_set readsCopy = m_reads, writesCopy = m_writes;
        int retval = select(m_socket + 1, &readsCopy, &writesCopy, 0, &peekInterval);

		if (retval == -1)
		{
			int errorCode = WSAGetLastError();
			m_GameShared.ErrorLog("select failed. error code - {}");
		}
		else if (retval)
		{
			// Data is available now.
			for (int k = 0; k < m_socket + 1; ++k)
			{
				if (FD_ISSET(k, &readsCopy))
				{
					int readBytes = recv(k, m_inputData.m_buffer, BUFFER_SIZE, 0);
					bool ret = m_GameShared.OnReceived(m_inputData, readBytes);

					FD_CLR(m_socket, &readsCopy);
				}

				if (FD_ISSET(k, &writesCopy))
				{
					FD_CLR(m_socket, &writesCopy); 
				}
			}
		}

		FD_ZERO(&m_reads);
		return m_keepLooping.load();
	}
	int GBGameClient::Connect(const std::string_view address, const int port)
	{
		FD_ZERO(&m_reads);
		FD_ZERO(&m_writes);

		if (int result = __super::Connect(address, port))
		{
			return result;
		}

		FD_SET(m_socket, &m_reads);
		FD_SET(m_socket, &m_writes);
		return 0;
	}
}