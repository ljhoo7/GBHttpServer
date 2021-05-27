#include "../include/stdafx.h"
#include "../include/GBExpandedOverlapped.h"

namespace GenericBoson
{
	bool GBExpandedOverlapped::Parse()
	{
		std::string token;

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
					m_parsed.push_back(token);
					token.clear();
					return true;
				}
				CRReadJustBefore = false;
				break;
			case ' ':
				CRReadJustBefore = false;
				m_parsed.push_back(token);
				token.clear();
				break;
			default:
				CRReadJustBefore = false;
				token.push_back(iChar);
				break;
			}
		}

		return false;
	}
}