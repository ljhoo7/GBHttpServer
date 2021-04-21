#include "../include/stdafx.h"

#include "../include/GBHttpRouter.h"
#include "../include/GBHttpRequestLineReader.h"

namespace GenericBoson
{
	HttpVersion GBHttpRequestLineReader::Read(const std::string_view target, std::string_view targetPath)
	{
		bool parseResult = Parse(target);

		if (false == parseResult)
		{
			// #ToDo
			// Invalid line exists.
			return HttpVersion::None;
		}

		size_t parsedSize = m_parsed.size();

		if (2 == parsedSize)
		{
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