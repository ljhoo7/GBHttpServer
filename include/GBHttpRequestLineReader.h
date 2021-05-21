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

		// Gathering a request line.
		// \param target 
		// \param targetPath
		// \param methodName
		// \return bool - Is this gathering completed ?, HttpVersion - The HTTP version of this request line.
		std::pair<bool, HttpVersion> Read(const std::string_view target, std::string& targetPath, std::string& methodName);
	};
}