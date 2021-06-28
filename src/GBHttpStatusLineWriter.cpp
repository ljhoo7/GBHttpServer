#include "../include/stdafx.h"

#include "../include/GBHttpStatusLineWriter.h"

namespace GenericBoson
{
	// Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	GBHttpStatusLineWriter::GBHttpStatusLineWriter(const HttpVersion version, const GBHttpResponse & response, const std::string& reason)
	{
		std::stringstream sstream;

		//sstream << version << ' ' << response << ' ' << reason << '\r\n';
	}
}
