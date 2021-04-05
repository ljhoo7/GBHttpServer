#pragma once

#include <vector>

#include "../lib/GBString/include/GBString.h"
#include "GBMethod.h"

namespace GenericBoson
{
	class GBMethod;

	class GBHttpRouter
	{
	protected:
		const SOCKET& m_acceptedSocket;
		std::vector<GBMethod> m_methodList;
	public:
		GBHttpRouter(const SOCKET& accpetedSocket) : m_acceptedSocket(accpetedSocket) {}
		virtual ~GBHttpRouter() = default;
		virtual bool Route(const GBStringView subStr) = 0;
	};
}