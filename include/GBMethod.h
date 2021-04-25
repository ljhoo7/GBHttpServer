#pragma once

#include <functional>
#include "../lib/GBString/include/GBString.h"

namespace GenericBoson
{
	/*
	http에서 GET, PUT, POST 같은 것을 뜻합니다.
	*/
	struct GBMethod
	{
		std::function<void(int)> m_method;

		GBMethod() = default;
		virtual ~GBMethod() = default;

		GBMethod(std::function<void(int)>& method)
			: m_method(method) {}

		virtual std::string GetName() = 0;
	};
}