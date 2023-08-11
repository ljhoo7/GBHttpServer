#include "../include/stdafx.h"

#include "../include/GBHttpReader.h"

namespace GenericBoson
{
	HTTP_STATUS_CODE GBHttpReader::ParseAndRead(GBHttpInformation* pOutInfo)
	{
		HTTP_STATUS_CODE parseResult = ParseToken(pOutInfo);

		if (HTTP_STATUS_CODE::OK != parseResult)
		{
			return parseResult;
		}

		return Read(pOutInfo);
	}
}