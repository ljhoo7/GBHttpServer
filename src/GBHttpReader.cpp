#include "../include/stdafx.h"

#include "../include/GBHttpReader.h"

namespace GenericBoson
{
	bool GBHttpReader::ParseAndRead(GBHttpInformation* pOutInfo)
	{
		ParseToken();

		return Read(pOutInfo);
	}
}