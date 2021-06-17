#include "../include/stdafx.h"

#include "../include/GBHttpHeaderReader.h"

namespace GenericBoson
{
	bool GBHttpHeaderReader::Read(GBHttpInformation* pOutInfo)
	{

		return true;
	}

	void GBHttpHeaderReader::ParseToken()
	{
		size_t findResult = std::string::npos;
		while (std::string::npos != (findResult = m_lines.front().find_first_of(':')))
		{
			std::string_view frontLine = m_lines.front();

			// RFC 명세상 ':' 다음에 반드시 빈칸한칸이 와야한다.
			assert(' ' == frontLine[findResult + 1]);

			std::string_view headerName = frontLine.substr(0, findResult);
			std::string_view headerValue = frontLine.substr(findResult + 2); // ':'과 ' '를 건너뛴다.

			m_headerMap.emplace(headerName, headerValue);

			m_lines.pop();
		}
	}
}