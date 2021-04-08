#pragma once

#include <vector>

#include "../lib/GBString/include/GBString.h"
#include "GBResponse.h"
#include "GBPathSegment.h"

namespace GenericBoson
{
	class GBHttpRouter
	{
	protected:
		const SOCKET& m_acceptedSocket;
		std::vector<PathSegment> m_methodList;
	public:
		GBHttpRouter(const SOCKET& acceptedSocket) : m_acceptedSocket(acceptedSocket) {}
		virtual ~GBHttpRouter() = default;
		virtual bool Route(const GBStringView subStr) = 0;
	};
}