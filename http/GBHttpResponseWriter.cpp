#include "stdafx.h"

#include "GBHttpResponseWriter.h"

namespace GenericBoson
{
	GBHttpResponseWriter::GBHttpResponseWriter(GBExpandedOverlapped* pEol)
		: m_pEol(pEol)
	{
		pEol->m_sendOffset = 0;
	}

	GBHttpResponseWriter::~GBHttpResponseWriter()
	{
		int issueSendResult = IssueSend();
	}

	int GBHttpResponseWriter::IssueSend()
	{
		WSABUF bufToSend;
		DWORD sentBytes = 0;
		bufToSend.buf = m_pEol->m_sendBuffer;
		bufToSend.len = m_pEol->m_sendOffset;
		int sendResult = WSASend(m_pEol->m_socket, &bufToSend, 1, &sentBytes, 0, m_pEol, nullptr);

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

		// RFC 7230에 HTTP 메세지란 다음과 같은 형태라고 나와있다.
		/*
		HTTP-message = start-line
					  *( header-field CRLF )
					  CRLF
					  [ message-body ]
		*/
		// 여기에서 엄청 주의할 점은 헤더필드 리스트와 메세지 바디 사이의 CRLF이다.
		// 모든 헤더-필드 쌍은 CRLF로 끝나는데 그러한 쌍들의 마지막에 CRLF가 한번 더 붙어야 제대로된 메세지라는 뜻이다.
		// 이 모양새를 대충보고 HTTP message writer를 구현한다면,
		// 헤더-필드쌍들을 다 쓰고 바로 메세지 바디를 이어서 써버릴 수도 있다.
		// 그렇게 하면 Http Client들이 무한행이 걸리거나 잘못된 메세지라고 뱉어버리게 된다. 왜 그러는지 이유도 알려주지 않아서 디버깅이 곤란해진다.
		// 속 편하게 헤더-필드 리스트가 끝나면 개행이 반드시 "두번"되고 바디가 붙는다고 이해하면 편하다.
		// 물론 CRLF가 2번이면 더 좋지만 LF만 두번해도 제대로 동작하는 듯하다.
		WriteOneLineToBuffer("\r\n");

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
			"<h1>Page1</h1>\n"
			"<a href=\"/page2\">->page2</a>\n"
			"</body>"
			"</html>"
		);

		if (false == ret)
		{
			return false;
		}

		return true;
	}

	bool GBHttpResponseWriter::WriteOneLineToBuffer(const char* format, ...)
	{
		char* pLineStartPosition = &m_pEol->m_sendBuffer[m_pEol->m_sendOffset];

		va_list argList;
		__crt_va_start(argList, format);
		int writtenCountOrErrorCode = _vsprintf_s_l(pLineStartPosition, BUFFER_SIZE - m_pEol->m_sendOffset, format, NULL, argList);
		__crt_va_end(argList);

		if (-1 == writtenCountOrErrorCode)
		{
			return false;
		}

		m_lines.emplace_back(pLineStartPosition, writtenCountOrErrorCode);

		m_pEol->m_sendOffset += writtenCountOrErrorCode;

		return true;
	}
}
