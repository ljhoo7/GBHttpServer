#include "stdafx.h"

#include "GBHttpRequestLineReader.h"

namespace GenericBoson
{
	GBHttpRequestLineReader::GBHttpRequestLineReader(std::queue<std::string_view>& lines)
	: m_requestLineCandidate(lines.front())
	{
		lines.pop();
	}

	HTTP_STATUS_CODE GBHttpRequestLineReader::ParseToken(GBHttpInformation* pOutInfo)
	{
		size_t findResult = 0, prevIndex = 0;
		while (std::string::npos != findResult)
		{
			findResult = m_requestLineCandidate.find_first_of(' ', prevIndex);

			std::string_view token = std::string_view(m_requestLineCandidate).substr(prevIndex, findResult - prevIndex);

			m_tokens.push_back(token);

			prevIndex = findResult + 1;
		}

		return HTTP_STATUS_CODE::OK;
	}

	HTTP_STATUS_CODE GBHttpRequestLineReader::Read(GBHttpInformation* pOutInfo)
	{
		auto pOutRequestLineInfo = static_cast<GBHttpRequestLineInformation*>(pOutInfo);

		size_t parsedSize = m_tokens.size();

		if (2 == parsedSize)
		{
			pOutRequestLineInfo->m_methodName = m_tokens[0];
			pOutRequestLineInfo->m_targetPath = m_tokens[1];
			pOutRequestLineInfo->m_version = HttpVersion::Http09;
			return HTTP_STATUS_CODE::OK;
		}
		else if (3 == parsedSize)
		{
			std::string_view httpStr("HTTP/");
			// 아래 if는 원래 false == m_tokens[2].starts_with(httpStr) 이었지만,
			// travis windows가 VS2017까지만 지원해서 아래처럼 바꿈.
			if (httpStr != m_tokens[2].substr(0, httpStr.size()))
			{
				// #ToDo
				// Invalid request-line.
				return HTTP_STATUS_CODE::OK;
			}

			pOutRequestLineInfo->m_methodName = m_tokens[0];
			pOutRequestLineInfo->m_targetPath = m_tokens[1];
			std::string_view versionString(m_tokens[2]);
			std::string_view versionNumber = versionString.substr(httpStr.size());

			if ("0.9" == versionNumber)
			{
				pOutRequestLineInfo->m_version = HttpVersion::Http09;
				return HTTP_STATUS_CODE::OK;
			}
			else if ("1.0" == versionNumber)
			{
				pOutRequestLineInfo->m_version = HttpVersion::Http10;
				return HTTP_STATUS_CODE::OK;
			}
			else if ("1.1" == versionNumber)
			{
				pOutRequestLineInfo->m_version = HttpVersion::Http11;
				return HTTP_STATUS_CODE::OK;
			}
		}

		// #ToDo
		// Invalid request-line.
		return HTTP_STATUS_CODE::BAD_REQUEST;
	}
}