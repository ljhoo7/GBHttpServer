#include "../include/stdafx.h"

#include "../include/GBHttpResponseWriter.h"

namespace GenericBoson
{
	// Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	bool GBHttpResponseWriter::WriteStatusLine(const HttpVersion version, const GBHttpResponse & response, const std::string& reason)
	{
		std::stringstream sstream;

		float versionFloat = ((int)version) / 10.0f;

		sstream.precision(2);

		int statusCodeInteger = (int)response.GetStatusCode();

		sstream << "HTTP/" << versionFloat << ' ' << statusCodeInteger << ' ' << Constant::g_cStatusCodeToReasonPhaseMap.at(statusCodeInteger) << '\r' << '\n';

		m_lines.push_back(sstream.str());

		return true;
	}
}
