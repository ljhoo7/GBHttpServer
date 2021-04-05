#include "../include/stdafx.h"

#include "../include/GBHttpRequestLineReader.h"

namespace GenericBoson
{
	bool GBHttpRequestLineReader::Read(std::string_view target)
	{
		bool ret = Parse(target);

		if (false == ret)
		{
			// Invalid line exists.
			return false;
		}

		size_t parsedSize = m_parsed.size();

		if (2 == parsedSize)
		{

		}
		else if (3 == parsedSize)
		{

		}
		else
		{
			// Invalid request-line.
			return false;
		}
	}
}