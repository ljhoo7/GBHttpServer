#pragma once

#include "GBHttpReader.h"

namespace GenericBoson
{
	struct GBHeaderInformation : public GBHttpInformation
	{
		std::map<std::string, std::string> m_headerMap;
	};

	class GBHttpHeaderReader : public GBHttpReader
	{
		const std::vector<std::string>& m_cLines;

		// Inherited via GBHttpReader
		virtual std::pair<bool, GBHttpInformation> Read() override;
		virtual void ParseToken() override;

	public:
		GBHttpHeaderReader(const std::vector<std::string>& lines)
			: m_cLines(lines), GBHttpReader(lines) {}

		std::pair<bool, GBHeaderInformation> ParseAndRead();
	};
}