#include "../include/stdafx.h"

#include "../include/GBHttpResponseWriter.h"

namespace GenericBoson
{
	GBHttpResponseWriter::GBHttpResponseWriter(GBExpandedOverlapped* pEol)
		: m_pEol(pEol), m_wholeBufferStringView(pEol->m_buffer, BUFFER_SIZE)
	{
		pEol->m_offset = 0;
	}

	GBHttpResponseWriter::~GBHttpResponseWriter()
	{
		int issueSendResult = IssueSend();
	}

	int GBHttpResponseWriter::IssueSend()
	{
		WSABUF bufToSend;
		bufToSend.buf = m_pEol->m_buffer;
		bufToSend.len = m_pEol->m_leftBytesToTransfer;
		int sendResult = WSASend(m_pEol->m_socket, &bufToSend, 1, nullptr, 0, m_pEol, nullptr);
		return -1;
	}

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
