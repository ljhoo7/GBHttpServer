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

		}
	}
}