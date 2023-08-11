#pragma once

#include "GBHttpReader.h"
#include "GBHttpVersionTypes.h"

namespace GenericBoson
{
	struct GBHttpRequestLineInformation final : public GBHttpInformation
	{
		std::string_view m_methodName;
		std::string_view m_targetPath;
		std::map<std::string_view, std::string_view> m_queryMap;
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
		// \return bool - succeeded? GBHttpRequestLineInformation - The information of this request line.
		HTTP_STATUS_CODE Read(GBHttpInformation* pOutInfo) override;

		HTTP_STATUS_CODE ParseToken(GBHttpInformation* pOutInfo) override;
	public:
		GBHttpRequestLineReader(std::queue<std::string_view>& lines);
		virtual ~GBHttpRequestLineReader() = default;
	};
}