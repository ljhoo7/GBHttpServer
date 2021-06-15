#pragma once

#include "GBHttpReader.h"

namespace GenericBoson
{
	struct GBHttpHeaderInformation : public GBHttpInformation
	{
		std::map<std::string, std::string> m_headerMap;
	};

	class GBHttpHeaderReader : public GBHttpReader
	{
		const std::vector<std::string>& m_cLines;

		// Inherited via GBHttpReader
		virtual bool Read(GBHttpInformation* pOutInfo) override;

		virtual void ParseToken() override;

	public:
		GBHttpHeaderReader(const std::vector<std::string>& lines)
			: m_cLines(lines), GBHttpReader(lines) {}
	};
}