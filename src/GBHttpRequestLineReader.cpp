#include "../include/stdafx.h"

#include "../include/GBHttpRouter.h"
#include "../include/GBHttpRequestLineReader.h"

namespace GenericBoson
{
	GBHttpRequestLineReader::GBHttpRequestLineReader(const std::string_view requestLineCandidate)
	: m_requestLineCandidate(requestLineCandidate){}

	std::pair<bool, RequestLineInformation> GBHttpRequestLineReader::ParseAndRead()
	{
		bool succeeded = false;
		std::string parseResult;
		RequestLineInformation info;

		ParseToken();

		std::tie(succeeded, info) = Read();

		if (false == succeeded)
		{
			return { false, info };
		}

		return { true, info };
	}

	void GBHttpRequestLineReader::ParseToken()
	{
		size_t findResult = 0, prevIndex = 0;
		while (std::string::npos != findResult)
		{
			findResult = m_requestLineCandidate.find_first_of(' ', prevIndex);

			std::string_view token = m_requestLineCandidate.substr(prevIndex, findResult - prevIndex);

			m_tokens.push_back(token);

			prevIndex = findResult + 1;
		}
	}

	std::pair<bool, RequestLineInformation> GBHttpRequestLineReader::Read()
	{
		RequestLineInformation info;

		size_t parsedSize = m_tokens.size();

		if (2 == parsedSize)
		{
			info.m_methodName = m_tokens[0];
			info.m_targetPath = m_tokens[1];
			info.m_version = HttpVersion::Http09;
			return { true, info };
		}
		else if (3 == parsedSize)
		{
			std::string_view httpStr("HTTP/");
			// 아래 if는 원래 false == m_tokens[2].starts_with(httpStr) 이었지만,
			// travis windows가 VS2017까지만 지원해서 아래처럼 바꿈.
			if (httpStr != m_tokens[2].substr(0, httpStr.size()))
			{
				// #ToDo
				// Invalid request-line.
				return { false, info };
			}

			info.m_methodName = m_tokens[0];
			info.m_targetPath = m_tokens[1];
			std::string_view versionString(m_tokens[2]);
			std::string_view versionNumber = versionString.substr(httpStr.size());

			if ("0.9" == versionNumber)
			{
				info.m_version = HttpVersion::Http09;
				return { true , info };
			}
			else if ("1.0" == versionNumber)
			{
				info.m_version = HttpVersion::Http10;
				return { true , info };
			}
			else if ("1.1" == versionNumber)
			{
				info.m_version = HttpVersion::Http11;
				return { true , info };
			}
		}

		// #ToDo
		// Invalid request-line.
		return { false, info };
	}
}