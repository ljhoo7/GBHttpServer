#pragma once

namespace GenericBoson
{
	struct GBHttpInformation
	{

	};

	class GBHttpReader
	{
		// Read parsed tokens.
		// \return bool - succeeded? GBRequestLineInformation - The information of this request line.
		virtual std::pair<bool, GBHttpInformation> Read() = 0;
		virtual void ParseToken() = 0;

	public:

		GBHttpReader(const std::vector<std::string>& lines) {};
		virtual ~GBHttpReader() = default;

		// \return bool - succeeded? GBRequestLineInformation - The information of this request line.
		std::pair<bool, GBHttpInformation> ParseAndRead();
	};
}