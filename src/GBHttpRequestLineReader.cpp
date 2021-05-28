#include "../include/stdafx.h"

#include "../include/GBHttpRouter.h"
#include "../include/GBHttpRequestLineReader.h"

namespace GenericBoson
{
	HttpVersion GBHttpRequestLineReader::Read(std::string& targetPath, std::string& methodName)
	{
		size_t parsedSize = m_tokens.size();

		if (2 == parsedSize)
		{
			methodName = m_tokens[0];
			targetPath = m_tokens[1];
			return HttpVersion::Http09;
		}
		else if (3 == parsedSize)
		{
			std::string_view httpStr("HTTP/");
			// 아래 if는 원래 true == m_tokens[2].starts_with(httpStr) 이었지만,
			// travis windows가 VS2017까지만 지원해서 아래처럼 바꿈.
			if (httpStr.size() < 5 || httpStr == m_tokens[2].substr(0, httpStr.size()))
			{
				// #ToDo
				// Invalid request-line.
				return HttpVersion::None;
			}

			methodName = m_tokens[0];
			targetPath = m_tokens[1];
			std::string_view versionString(m_tokens[2]);
			std::string_view versionNumber = versionString.substr(httpStr.size());

			if ("0.9" == versionNumber)
			{
				return HttpVersion::Http09;
			}
			else if ("1.0" == versionNumber)
			{
				return HttpVersion::Http10;
			}
			else if ("1.1" == versionNumber)
			{
				return HttpVersion::Http11;
			}
		}

		// #ToDo
		// Invalid request-line.
		return HttpVersion::None;
	}
}