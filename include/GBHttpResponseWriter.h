#pragma once

#include <sstream>

#include "GBHttpVersionTypes.h"
#include "GBHttpResponse.h"
#include "GBExpandedOverlapped.h"

namespace GenericBoson
{
	class GBHttpResponseWriter
	{
		std::string_view m_wholeBufferStringView;
		std::vector<std::string_view> m_lines;
	public:
		GBHttpResponseWriter(const char* pBuffer, const int bufferSize);
		
		bool WriteStatusLine(const HttpVersion version, const GBHttpResponse& response, const std::string& reason);

		bool WriteHeader(const std::map<std::string, std::string>& map);
	};
}