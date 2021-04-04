#pragma once

#include "GBHttpLineReader.h"

namespace GenericBoson
{
	/*
	Read the first line of request.
	*/
	class GBHttpRequestLineReader : public GBHttpLineReader
	{
	public:
		GBHttpRequestLineReader() = default;
		virtual ~GBHttpRequestLineReader() = default;

		bool Read(std::string_view);
	};
}