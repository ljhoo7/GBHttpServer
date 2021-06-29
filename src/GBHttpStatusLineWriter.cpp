#include "../include/stdafx.h"

#include "../include/GBHttpStatusLineWriter.h"

namespace GenericBoson
{
	// Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	GBHttpStatusLineWriter::GBHttpStatusLineWriter(const HttpVersion version, const GBHttpResponse & response, const std::string& reason)
	{
		std::stringstream sstream;

		float versionFloat = ((int)version) / 10.0f;

		sstream.precision(2);

		sstream << "HTTP/" << versionFloat << ' ' << (int)response.GetStatusCode() << ' ' << reason << '\r' << '\n';

		m_lines.push_back(sstream.str());
	}
}
