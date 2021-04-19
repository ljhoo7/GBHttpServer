#include "../include/stdafx.h"

#include "../include/GBHttpRouter.h"
#include "../include/GBHttpRequestLineReader.h"

namespace GenericBoson
{
	HttpVersion GBHttpRequestLineReader::Read(const std::string_view target)
	{
		bool parseResult = Parse(target);

		if (false == parseResult)
		{
			// #ToDo
			// Invalid line exists.
			return false;
		}

		size_t parsedSize = m_parsed.size();

		if (2 == parsedSize)
		{
			return GBHttp09;
		}
		else if (3 == parsedSize)
		{
			std::string_view httpStr("HTTP/");
			if (false == m_parsed[2].starts_with(httpStr))
			{
				// #ToDo
				// Invalid request-line.
				return false;
			}

			std::string_view versionString(m_parsed[2]);
			std::string_view versionNumber = versionString.substr(httpStr.size());

			if ("0.9" == versionNumber)
			{
				m_pRouter = std::make_unique<GBHttpRouter<GBHttp09>>(m_acceptedSocket);
			}
			else if ("1.0" == versionNumber)
			{
				m_pRouter = std::make_unique<GBHttpRouter<GBHttp10>>(m_acceptedSocket);
			}
			else if ("1.1" == versionNumber)
			{
				m_pRouter = std::make_unique<GBHttpRouter<GBHttp11>>(m_acceptedSocket);
			}
		}
		else
		{
			// #ToDo
			// Invalid request-line.
			return false;
		}

		return true;
	}
}