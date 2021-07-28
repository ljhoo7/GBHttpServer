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
		/*WSABUF bufToSend;
		DWORD sentBytes = 0;
		bufToSend.buf = m_pEol->m_buffer;
		bufToSend.len = m_pEol->m_offset;
		int sendResult = WSASend(m_pEol->m_socket, &bufToSend, 1, &sentBytes, 0, m_pEol, nullptr);
		return sendResult;*/

		// HTTP
		const char* header =
			"HTTP/1.0 200 OK\n"
			"Content-type: text/html\n";

		int sendResult = send(m_pEol->m_socket, header, strlen(header), 0);
		if (sendResult < 1)
		{
			std::cout << "send response header failed : " << WSAGetLastError() << '\n';

			return false;
		}
		
		char html[1024] = { 0, };

		strcpy(html,
			"<!DOCTYPE html>\n"
			"<html lang = \"ja\">\n"
			"<head>\n"
			"<meta charset = \"utf-8\">\n"
			"</head>\n"
			"<body>\n"
			"<h1>Page1</h1>\n"
			"<a href=\"/page2\">->page2</a>\n"
			"</body>"
			"</html>");

		// 응답（HTML을 보낸다
		sendResult = send(m_pEol->m_socket, html, strlen(html), 0);
		if (sendResult < 1)
		{
			std::cout << "send response body failed : " << WSAGetLastError() << '\n';

			return false;
		};

		return 0;
	}

	// Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	bool GBHttpResponseWriter::WriteStatusLine(const HttpVersion version, const GBHttpResponse & response, const std::string& reason)
	{
		float versionFloat = ((int)version) / 10.0f;

		int statusCodeInteger = (int)response.GetStatusCode();

		return WriteOneLineToBuffer("HTTP/%.2f %d %s\r\n", versionFloat, statusCodeInteger, Constant::g_cStatusCodeToReasonPhaseMap.at(statusCodeInteger).c_str());
	}

	bool GBHttpResponseWriter::WriteHeader(const std::vector<std::pair<std::string, std::string>>& headerList)
	{
		for(auto riter = headerList.rbegin(); riter != headerList.rend(); ++riter)
		{
			bool ret = WriteOneLineToBuffer("%s: %s\r\n", riter->first.c_str(), riter->second.c_str());
			if (false == ret)
			{
				return false;
			}
		}

		return true;
	}

	bool GBHttpResponseWriter::WriteBody()
	{
		bool ret = WriteOneLineToBuffer(
			"<!DOCTYPE html>\n"
			"<html lang = \"ja\">\n"
			"<head>\n"
			"<meta charset = \"utf-8\">\n"
			"</head>\n"
			"<body>\n"
			"<h1>Page2</h1>\n"
			"<a href=\"/page1\">->page1</a>\n"
			"</body>\n"
			"</html>\r\n"
		);

		if (false == ret)
		{
			return false;
		}

		return true;
	}

	bool GBHttpResponseWriter::WriteOneLineToBuffer(const char* format, ...)
	{
		char* pLineStartPosition = &m_pEol->m_buffer[m_pEol->m_offset];

		va_list argList;
		__crt_va_start(argList, format);
		int writtenCountOrErrorCode = _vsprintf_s_l(pLineStartPosition, BUFFER_SIZE - m_pEol->m_offset, format, NULL, argList);
		__crt_va_end(argList);

		if (-1 == writtenCountOrErrorCode)
		{
			return false;
		}

		m_lines.emplace_back(pLineStartPosition, writtenCountOrErrorCode);

		m_pEol->m_offset += writtenCountOrErrorCode;

		return true;
	}
}
