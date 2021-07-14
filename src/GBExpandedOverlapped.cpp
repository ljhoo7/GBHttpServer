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
		char* pLineStart = nullptr;
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

					state = PARSE_LINE_STATE::CRLF_READ;
					break;
				}

				state = PARSE_LINE_STATE::LF_READ;
				break;
			default:
				if (0 == stringOffset)
				{
					pLineStart = &m_buffer[k];
				}
				stringOffset++;

				state = PARSE_LINE_STATE::OTHER_READ;
				break;
			}
		}

		return PARSE_LINE_STATE::CRLF_READ == state;
	}
}