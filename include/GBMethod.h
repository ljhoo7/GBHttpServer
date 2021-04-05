#pragma once

#include <functional>
#include "../lib/GBString/include/GBString.h"

namespace GenericBoson
{
	// http에서 GET, PUT, POST 같은 것을 뜻한다.
	class GBMethod
	{
	public:
		// #ToDo
		// 템플릿으로 callable을 받지 못해서 아쉽다.
		// 추후 방법을 모색해보자.
		const std::function<void(const GBStringView)> m_method;
		const GBString m_methodName;

		GBMethod(GBStringView methodName, const std::function<void(const GBStringView)>& method)
			: m_method(method), m_methodName(methodName.data()){}
	};
}