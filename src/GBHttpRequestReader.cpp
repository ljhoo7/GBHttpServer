#include "../include/stdafx.h"

#include "../include/GBHttpRequestReader.h"

namespace GenericBoson
{
	bool GBHttpRequestReader::Read(std::queue<std::string>& lines)
	{
		assert(0 < lines.size());

		// ExtendedOverlapped.GatherAndParseLines에서 빠져나왔다는 것은 최소 1줄은 읽었다는 것이다.
		GBHttpRequestLineReader requestLineReader(lines);

		bool succeeded = requestLineReader.ParseAndRead(&m_requestLineInfo);

		GBHttpHeaderInformation headerInfo;
		if (1 < lines.size())
		{
			// 헤더읽기
			GBHttpHeaderReader headerReader(lines);

			succeeded = headerReader.ParseAndRead(&m_headerInfo);
		}

		return succeeded;
	}
}