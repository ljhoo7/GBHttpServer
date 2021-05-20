#include "../include/stdafx.h"

#include "../include/GBHttpRouter.h"
#include "../include/GBHttpRequestLineReader.h"

namespace GenericBoson
{
	HttpVersion GBHttpRequestLineReader::Read(const std::string_view target, std::string& targetPath, std::string& methodName)
	{
		bool parseResult = Parse(target);

		if (false == parseResult)
		{
			// Parse 함수에서 아직 CRLF를 못 만난 상태. 즉, gathering을 더해야 된다.
			return HttpVersion::StillLeftToReceive;
		}

		size_t parsedSize = m_parsed.size();

		if (2 == parsedSize)
		{
			methodName = m_parsed[0];
			targetPath = m_parsed[1];
			return HttpVersion::Http09;
		}
		else if (3 == parsedSize)
		{
			std::string_view httpStr("HTTP/");
			if (false == m_parsed[2].starts_with(httpStr))
			{
				// #ToDo
				// Invalid request-line.
				return HttpVersion::None;
			}

			methodName = m_parsed[0];
			targetPath = m_parsed[1];
			std::string_view versionString(m_parsed[2]);
			std::string_view versionNumber = versionString.substr(httpStr.size());

			if ("0.9" == versionNumber)
			{
				return HttpVersion::Http09;;
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