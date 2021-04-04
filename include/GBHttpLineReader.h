#pragma once

namespace GenericBoson
{
	class GBHttpLineReader
	{
		bool Parse(const std::string_view);
	public:
		GBHttpLineReader() = default;
		virtual ~GBHttpLineReader() = default;
	};
}