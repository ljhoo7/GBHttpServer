#pragma once

#include "GBHttpReader.h"

namespace GenericBoson
{
	struct GBHttpHeaderInformation final : public GBHttpInformation
	{
		std::map<std::string, std::string> m_headerMap;
	};

	class GBHttpHeaderReader : public GBHttpReader
	{
		std::queue<std::string>& m_lines;

		// Inherited via GBHttpReader
		virtual bool Read(GBHttpInformation* pOutInfo) override;

		virtual void ParseToken() override;

	public:
		GBHttpHeaderReader(std::queue<std::string>& lines)
			: m_lines(lines), GBHttpReader(lines) {}
	};
}