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
		virtual ~GBHttpRequestLineReader() = default;

		HttpVersion Read(const std::string_view, std::string_view targetPath, std::string_view methodName);
	};
}