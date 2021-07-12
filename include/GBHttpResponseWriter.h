#pragma once

#include <sstream>

#include "GBHttpVersionTypes.h"
#include "GBHttpResponse.h"
#include "GBExpandedOverlapped.h"

namespace GenericBoson
{
	class GBHttpResponseWriter
	{
		std::vector<std::string> m_lines;
	public:
		GBHttpResponseWriter() = default;
		
		bool WriteStatusLine(const HttpVersion version, const GBHttpResponse& response, const std::string& reason);

		bool WriteHeader(const std::map<std::string, std::string>& map);

		bool LoadToBuffer(const GBExpandedOverlapped* pEol);
	};
}