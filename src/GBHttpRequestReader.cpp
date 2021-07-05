#include "../include/stdafx.h"

#include "../include/GBHttpRequestReader.h"

namespace GenericBoson
{
	bool GBHttpRequestReader::Read(std::queue<std::string>& lines)
	{
		assert(0 < lines.size());

		bool succeeded = true;
		while (0 < m_readerToInformationQueue.size())
		{
			auto& iPair = m_readerToInformationQueue.front();

			succeeded &= iPair.first->ParseAndRead(iPair.second.get());

			m_readerToInformationQueue.pop();
		}

		return succeeded;
	}
}