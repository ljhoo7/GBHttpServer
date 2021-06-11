#pragma once

namespace GenericBoson
{
	struct GBHeaderInformation
	{
		std::map<std::string, std::string> m_headerMap;
	};

	class GBHttpHeaderReader
	{
		const std::vector<std::string>& m_cLines;

	public:
		GBHttpHeaderReader(const std::vector<std::string>& lines)
			: m_cLines(lines) {}

		std::pair<bool, GBHeaderInformation> ParseAndRead();
	};
}