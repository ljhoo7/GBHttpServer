#pragma once

#include <sstream>
#include <cstdarg>

#include "GBHttpVersionTypes.h"
#include "GBHttpResponse.h"
#include "GBExpandedOverlapped.h"

namespace GenericBoson
{
	class GBHttpResponseWriter final
	{
		GBExpandedOverlapped* m_pEol = nullptr;
		std::vector<std::string_view> m_lines;
	public:
		GBHttpResponseWriter(GBExpandedOverlapped* pEol);
		~GBHttpResponseWriter();
		
		bool WriteStatusLine(const HttpVersion version, const GBHttpResponse& response, const std::string& reason);

		bool WriteHeader(const std::map<std::string, std::string>& map);

		int IssueSend();
	};
}