#include "../include/stdafx.h"

#include "../include/GBHttpReader.h"

namespace GenericBoson
{
	std::pair<bool, GBHttpInformation> GBHttpReader::ParseAndRead()
	{
		bool succeeded = false;
		std::string parseResult;
		GBRequestLineInformation info;

		ParseToken();

		std::tie(succeeded, info) = Read();

		if (false == succeeded)
		{
			return { false, info };
		}

		return { true, info };
	}
}