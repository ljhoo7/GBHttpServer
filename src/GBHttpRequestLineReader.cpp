#include "../include/stdafx.h"

#include "../include/GBHttp1.0Router.h"
#include "../include/GBHttpRequestLineReader.h"

namespace GenericBoson
{
	bool GBHttpRequestLineReader::Read(const GBStringView target)
	{
		bool ret = Parse(target);

		if (false == ret)
		{
			// Invalid line exists.
			return false;
		}

		size_t parsedSize = m_parsed.size();

		std::unique_ptr<GBHttpRouter> pRouter = nullptr;

		if (2 == parsedSize)
		{
			pRouter = std::make_unique<GBHttp09Router>(m_acceptedSocket);
		}
		else if (3 == parsedSize)
		{
			if (false == m_parsed[2].starts_with(_T("HTTP")))
			{

			}

			pRouter = std::make_unique<GBHttp10Router>(m_acceptedSocket);
		}
		else
		{
			// Invalid request-line.
			return false;
		}

		return true;
	}
}