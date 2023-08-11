#include "../include/stdafx.h"
#include "../include/GBExpandedOverlapped.h"

namespace GenericBoson
{
	bool GBExpandedOverlapped::GatherAndParseLines(DWORD receivedBytes)
	{
		int stringOffset = 0;
		// All Http message ( except for Entity-Body ) must be ended by CRLF or LF.
		int k = m_recvOffset;
		char* pLineStart = nullptr;
		for (; k < m_recvOffset + receivedBytes; ++k)
		{
			switch (m_recvBuffer[k])
			{
			case '\r':
				// carrage return은 무시한다.
				// RFC 문서에 의하면 HTTP message의 request line과 header-value list들은 개항을 CRLF로 해야 된다고 나왔있다.
				// 그러나, 'HTTP 완벽 가이드'책에 의하면, 옛날 프로그램 중 CR 없이 LF만으로 개행하는 것들이 많다고 한다.
				break;
			case '\n':
				m_lines.emplace(pLineStart, stringOffset);
				stringOffset = 0;
				break;
			default:
				if (0 == stringOffset)
				{
					pLineStart = &m_recvBuffer[k];
				}
				stringOffset++;
				break;
			}
		}

		return 0 == stringOffset;
	}
}