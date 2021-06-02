#include "../include/stdafx.h"
#include "../include/GBExpandedOverlapped.h"

namespace GenericBoson
{
	bool GBExpandedOverlapped::GatherAndParseLines()
	{
		std::string line;

		// All Http message ( except for Entity-Body ) must be ended by CRLF.
		bool CRReadJustBefore = false;
		for (auto iChar : m_buffer)
		{
			switch (iChar)
			{
			case '\r':
				CRReadJustBefore = true;
				break;
			case '\n':
				if (true == CRReadJustBefore)
				{
					m_lines.push_back(line);
					line.clear();
					break;
				}
				CRReadJustBefore = false;
				break;
			default:
				CRReadJustBefore = false;
				line.push_back(iChar);
				break;
			}
		}

		return CRReadJustBefore;
	}
}