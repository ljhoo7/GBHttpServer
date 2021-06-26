#pragma once

#include "GBExpandedOverlapped.h"

namespace GenericBoson
{
	class GBHttpStatusLineWriter
	{
		std::vector<std::string> m_lines;
	public:
		GBHttpStatusLineWriter(const GBExpandedOverlapped* pEol);
	};
}