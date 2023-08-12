#include "stdafx.h"

#include "GBHttpRequestReader.h"

namespace GenericBoson
{
	HTTP_STATUS_CODE GBHttpRequestReader::Read()
	{
		while (0 < m_readerToInformationQueue.size())
		{
			auto& iPair = m_readerToInformationQueue.front();

			HTTP_STATUS_CODE readResult = iPair.first->ParseAndRead(iPair.second.get());

			if (HTTP_STATUS_CODE::OK != readResult)
			{
				// #ToDo
				return readResult;
			}

			m_readerToInformationQueue.pop();
		}

		return HTTP_STATUS_CODE::OK;
	}
}