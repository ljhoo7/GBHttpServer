#pragma once

#include <queue>
#include "Constant.h"

namespace GenericBoson
{
	struct GBHttpInformation
	{
		virtual ~GBHttpInformation() = default;
	};

	class GBHttpReader
	{
		// Read parsed tokens.
		// \return bool - succeeded? GBHttpRequestLineInformation - The information of this request line.
		virtual HTTP_STATUS_CODE Read(GBHttpInformation* pOutInfo) = 0;

		virtual HTTP_STATUS_CODE ParseToken() = 0;

	public:

		GBHttpReader() = default;
		virtual ~GBHttpReader() = default;

		// \GBHttpRequestLineInformation - The information of this request line.
		// \return bool - succeeded? 
		virtual HTTP_STATUS_CODE ParseAndRead(GBHttpInformation* pOutInfo) final;
	};
}