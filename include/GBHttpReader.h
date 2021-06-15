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
		virtual bool Read(GBHttpInformation* pOutInfo) = 0;

		virtual void ParseToken() = 0;

	public:

		GBHttpReader(const std::vector<std::string>& lines) {};
		virtual ~GBHttpReader() = default;

		// \GBRequestLineInformation - The information of this request line.
		// \return bool - succeeded? 
		bool ParseAndRead(GBHttpInformation* pOutInfo);
	};
}