#pragma once

#include "GBHttpReader.h"
#include "GBHttpMessageBody.h"

namespace GenericBoson
{
	class GBExpandedOverlapped;

	class GBHttpBodyReader : public GBHttpReader
	{
		// Inherited via GBHttpReader
		virtual HTTP_STATUS_CODE Read(GBHttpInformation* pOutInfo) override;

		virtual HTTP_STATUS_CODE ParseToken(GBHttpInformation* pOutInfo) override;

	public:

		GBHttpBodyReader(const GBExpandedOverlapped* lines){}
	};
}