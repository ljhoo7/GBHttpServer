#pragma once

#include "GBHttpReader.h"

namespace GenericBoson
{
	struct GBHttpBodyInformation final : public GBHttpInformation
	{
		std::map<std::string_view, std::string_view> m_headerMap;
	};

	class GBHttpBodyReader : public GBHttpReader
	{
		std::queue<std::string_view>& m_lines;

		// Inherited via GBHttpReader
		virtual HTTP_STATUS_CODE Read(GBHttpInformation* pOutInfo) override;

		virtual HTTP_STATUS_CODE ParseToken(GBHttpInformation* pOutInfo) override;

	public:

		GBHttpBodyReader(std::queue<std::string_view>& lines)
			: m_lines(lines) {}
	};
}