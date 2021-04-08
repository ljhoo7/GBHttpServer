#pragma once

#include <functional>
#include "../lib/GBString/include/GBString.h"

namespace GenericBoson
{
	/*
	http에서 GET, PUT, POST 같은 것을 뜻합니다.
	*/
	template<typename METHOD>
	class GBMethod
	{
	public:
		const METHOD& m_method;

		GBMethod(const METHOD& method)
			: m_method(method) {}
	};
}