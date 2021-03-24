#pragma once

#include <functional>

namespace GenericBoson
{
	// http에서 GET, PUT, POST 같은 것을 뜻한다.
	class GBMethod
	{
	public:
		// #ToDo
		// 템플릿으로 callable을 받지 못해서 아쉽다.
		// 추후 방법을 모색해보자.
		const std::function<void()> m_method;
		const std::string m_methodName;

		GBMethod(std::string_view methodName, const std::function<void()>& method)
			: m_method(method), m_methodName(methodName.data()){}
	};
}