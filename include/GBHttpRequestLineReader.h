#pragma once

#include "GBHttpReader.h"

namespace GenericBoson
{
	struct GBRequestLineInformation : public GBHttpInformation
	{
		std::string m_methodName;
		std::string m_targetPath;
		HttpVersion m_version = HttpVersion::None;
	};

	/*
	Read the first line of request.
	*/
	class GBHttpRequestLineReader : public GBHttpReader
	{
		const std::string_view m_requestLineCandidate;
		std::vector<std::string_view> m_tokens;

		// Read parsed tokens.
		// \return bool - succeeded? GBRequestLineInformation - The information of this request line.
		std::pair<bool, GBHttpInformation> Read() override;

		void ParseToken() override;
	public:
		GBHttpRequestLineReader(const std::vector<std::string>& lines);
		virtual ~GBHttpRequestLineReader() = default;

		// \return bool - succeeded? GBRequestLineInformation - The information of this request line.
		std::pair<bool, GBRequestLineInformation> ParseAndRead();
	};
}