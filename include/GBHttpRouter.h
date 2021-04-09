#pragma once

#include <map>
#include <vector>

#include "../lib/GBString/include/GBString.h"
#include "GBResponse.h"
#include "GBPathSegment.h"

namespace GenericBoson
{
	template<typename CALLABLE>
	class GBHttpRouter
	{
	protected:
		const SOCKET& m_acceptedSocket;
		std::map<std::string, PathSegment<CALLABLE>> m_methodTree;
	public:
		GBHttpRouter(const SOCKET& acceptedSocket) : m_acceptedSocket(acceptedSocket) {}
		virtual ~GBHttpRouter() = default;
		virtual bool Route(const GBStringView subStr) = 0;
	};
}