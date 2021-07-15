#include "../include/stdafx.h"

#include "../include/GBHttpResponseWriter.h"

namespace GenericBoson
{
	GBHttpResponseWriter::GBHttpResponseWriter(const char * pBuffer, const int bufferSize)
		: m_wholeBufferStringView(pBuffer, bufferSize){}

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

	bool GBHttpResponseWriter::LoadToBuffer(const GBExpandedOverlapped* pEol)
	{
		for (auto& iLine : m_lines)
		{
		}

		return true;
	}

	bool GBHttpResponseWriter::WriteHeader(const std::map<std::string, std::string>& map)
	{
		std::stringstream sstream;

		for (auto& iHeader : map)
		{
			sstream << iHeader.first << ':' << ' ' << iHeader.second << '/r' << '/n';
		}

		m_lines.push_back(sstream.str());

		return false;
	}
}
