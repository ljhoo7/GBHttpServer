#pragma once

#include "GBHttpResponse.h"
#include <functional>
#include <cstdarg>

namespace GenericBoson
{
	typedef std::function<GBHttpResponse(...)> GB_ACTION_METHOD;

	/*
	http에서 GET, PUT, POST 같은 것을 뜻합니다.
	*/
	struct GBMethod
	{
		GB_ACTION_METHOD m_method;

		GBMethod() = default;
		virtual ~GBMethod() = default;

		GBMethod(GB_ACTION_METHOD& method)
			: m_method(method) {}

		virtual std::string GetName() = 0;
	};
}