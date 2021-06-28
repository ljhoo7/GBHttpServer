#pragma once

#include <sstream>

#include "GBHttpVersionTypes.h"
#include "GBHttpResponse.h"
#include "GBExpandedOverlapped.h"

namespace GenericBoson
{
	class GBHttpStatusLineWriter
	{
		std::vector<std::string> m_lines;
	public:
		GBHttpStatusLineWriter(const HttpVersion version, const GBHttpResponse& response, const std::string& reason);
	};
}