#pragma once

#include "GBHttpResponse.h"
#include "GBHttpMessageBody.h"
#include <functional>
#include <cstdarg>

#define GB_ACTION_METHOD_PARAMETERS const std::map<std::string_view, std::string_view>&, const std::map<std::string_view, std::string_view>&, const GBHttpMessageBody&

namespace GenericBoson
{
	// Explanation about std::functino template parameter :
	// Parameters
	//		1. const std::map<std::string, std::string>& : 쿼리 키-값 리스트
	//		2. const std::map<std::string, std::string>& : 헤더-필드 쌍 리스트
	//		3. const GBHttpMessageBody& : HTTP message body 객체
	// Return Value
	//		1. GBHttpResponse : 보낼 response 객체
	typedef std::function<GBHttpResponse(GB_ACTION_METHOD_PARAMETERS)> GB_ACTION_METHOD;

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