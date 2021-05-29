#pragma once

#include "GBHttpLineReader.h"

namespace GenericBoson
{
	/*
	Read the first line of request.
	*/
	class GBHttpRequestLineReader : public GBHttpLineReader
	{
		std::vector<std::string> m_tokens;
	public:
		virtual ~GBHttpRequestLineReader() = default;

		// Gathering a request line.
		// \param targetPath
		// \param methodName
		// \return The HTTP version of this request line.
		HttpVersion Read(std::string& targetPath, std::string& methodName);
	};
}