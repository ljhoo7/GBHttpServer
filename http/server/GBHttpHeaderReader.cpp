#include "GBHttpHeaderReader.h"

namespace GenericBoson
{
	HTTP_STATUS_CODE GBHttpHeaderReader::Read(GBHttpInformation* pOutInfo)
	{
		auto pOutHeaderInfo = static_cast<GBHttpHeaderInformation*>(pOutInfo);

		return HTTP_STATUS_CODE::OK;
	}

	HTTP_STATUS_CODE GBHttpHeaderReader::ParseToken(GBHttpInformation* pOutInfo)
	{
		auto pOutHeaderInfo = static_cast<GBHttpHeaderInformation*>(pOutInfo);

		size_t findResult = std::string::npos;
		while (std::string::npos != (findResult = m_lines.front().find_first_of(':')))
		{
			std::string_view frontLine = m_lines.front();

			std::string_view headerName = frontLine.substr(0, findResult);
			std::string_view headerValue = frontLine.substr(findResult + 2); // ':'과 ' '를 건너뛴다.

			pOutHeaderInfo->m_headerMap.emplace(headerName, headerValue);

			m_lines.pop();
		}

		return HTTP_STATUS_CODE::OK;
	}
}