#include "../include/stdafx.h"

#include "../include/GBHttpResponseWriter.h"

namespace GenericBoson
{
	GBHttpResponseWriter::GBHttpResponseWriter(GBExpandedOverlapped* pEol)
		: m_pEol(pEol)
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
		bufToSend.len = m_pEol->m_offset;
		int sendResult = WSASend(m_pEol->m_socket, &bufToSend, 1, nullptr, 0, m_pEol, nullptr);
		return sendResult;
	}

	// Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	bool GBHttpResponseWriter::WriteStatusLine(const HttpVersion version, const GBHttpResponse & response, const std::string& reason)
	{
		float versionFloat = ((int)version) / 10.0f;

		int statusCodeInteger = (int)response.GetStatusCode();

		char* pLineStartPosition = &m_pEol->m_buffer[m_pEol->m_offset];

		int writtenCountOrErrorCode = sprintf_s(pLineStartPosition, BUFFER_SIZE, "HTTP/%.2f %d %s\r\n", versionFloat, statusCodeInteger, Constant::g_cStatusCodeToReasonPhaseMap.at(statusCodeInteger).c_str());

		if (-1 == writtenCountOrErrorCode)
		{
			return false;
		}

		m_lines.emplace_back(pLineStartPosition, writtenCountOrErrorCode);

		m_pEol->m_offset += writtenCountOrErrorCode;

		return true;
	}

	bool GBHttpResponseWriter::WriteHeader(const std::map<std::string, std::string>& map)
	{
		std::stringstream sstream;

		for (auto& iHeader : map)
		{
			char* pLineStartPosition = &m_pEol->m_buffer[m_pEol->m_offset];

			int writtenCountOrErrorCode = sprintf_s(pLineStartPosition, BUFFER_SIZE, "%s: %s\r\n", iHeader.first.c_str(), iHeader.second.c_str());

			if (-1 == writtenCountOrErrorCode)
			{
				return false;
			}

			m_lines.emplace_back(pLineStartPosition, writtenCountOrErrorCode);

			m_pEol->m_offset += writtenCountOrErrorCode;
		}

		return false;
	}
}
