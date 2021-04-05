#pragma once

#include "../lib/GBString/include/GBString.h"

namespace GenericBoson
{
	class GBHttpLineReader
	{
	protected:
		/*
		Tokens parsed by Parse function.
		*/
		std::vector<GBString> m_parsed;
	public:
		GBHttpLineReader() = default;
		virtual ~GBHttpLineReader() = default;

		/*
		Parse a line in the HTTP message.

		\return Was It succeeded?
		*/
		bool Parse(const GBStringView);
	};
}