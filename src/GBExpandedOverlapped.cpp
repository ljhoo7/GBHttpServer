#include "../include/stdafx.h"
#include "../include/GBExpandedOverlapped.h"

namespace GenericBoson
{
	bool GBExpandedOverlapped::GatherAndParseLines(DWORD receivedBytes)
	{
		int stringOffset = 0;
		// All Http message ( except for Entity-Body ) must be ended by CRLF.
		PARSE_LINE_STATE state = PARSE_LINE_STATE::OTHER_READ;
		int k = m_offset;
		char* pLineStart = &m_buffer[k];
		for (; k < m_offset + receivedBytes; ++k)
		{
			switch (m_buffer[k])
			{
			case '\r':
				state = PARSE_LINE_STATE::CR_READ;
				break;
			case '\n':
				if (PARSE_LINE_STATE::CR_READ == state)
				{
					m_lines.emplace(pLineStart, stringOffset);
					stringOffset = 0;
					pLineStart = &m_buffer[k];

					state = PARSE_LINE_STATE::CRLF_READ;
					break;
				}

				state = PARSE_LINE_STATE::LF_READ;
				break;
			default:
				stringOffset++;

				state = PARSE_LINE_STATE::OTHER_READ;
				break;
			}
		}

		return PARSE_LINE_STATE::CRLF_READ == state;
	}
}