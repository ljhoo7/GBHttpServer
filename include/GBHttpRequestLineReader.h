#pragma once

#include "GBHttpReader.h"
#include "GBHttpVersionTypes.h"

namespace GenericBoson
{
	struct GBHttpRequestLineInformation final : public GBHttpInformation
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
		const std::string m_requestLineCandidate;
		std::vector<std::string_view> m_tokens;

		// Read parsed tokens.
		// \return bool - succeeded? GBHttpRequestLineInformation - The information of this request line.
		bool Read(GBHttpInformation* pOutInfo) override;

		HTTP_STATUS_CODE ParseToken() override;
	public:
		GBHttpRequestLineReader(std::queue<std::string>& lines);
		virtual ~GBHttpRequestLineReader() = default;
	};
}