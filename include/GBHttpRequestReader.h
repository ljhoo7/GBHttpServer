#pragma once

#include "GBHttpRequestLineReader.h"
#include "GBHttpHeaderReader.h"

namespace GenericBoson
{
	class GBHttpRequestReader
	{
	public:
		GBHttpRequestReader() = default;

		GBRequestLineInformation m_requestLineInfo;
		GBHttpHeaderInformation m_headerInfo;

		bool Read(std::queue<std::string>& lines);
	};
}