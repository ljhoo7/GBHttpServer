#pragma once

namespace GenericBoson
{
	struct GBRequestLineInformation
	{
		std::string m_methodName;
		std::string m_targetPath;
		HttpVersion m_version = HttpVersion::None;
	};

	/*
	Read the first line of request.
	*/
	class GBHttpRequestLineReader
	{
		const std::string_view m_requestLineCandidate;
		std::vector<std::string_view> m_tokens;

		// Read parsed tokens.
		// \return bool - succeeded? GBRequestLineInformation - The information of this request line.
		std::pair<bool, GBRequestLineInformation> Read();

		void ParseToken();
	public:
		GBHttpRequestLineReader(const std::vector<std::string>& lines);
		virtual ~GBHttpRequestLineReader() = default;

		// \return bool - succeeded? GBRequestLineInformation - The information of this request line.
		std::pair<bool, GBRequestLineInformation> ParseAndRead();
	};
}