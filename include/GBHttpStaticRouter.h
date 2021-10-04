#pragma once

#include <string_view>

#define COMBINE_INTERNAL(X,Y) X##Y
#define COMBINE(X,Y) COMBINE_INTERNAL(X,Y)
#define PUT(PATH,CALLABLE) static const char COMBINE(METHOD,__LINE__)[]{ PATH };METHOD<COMBINE(METHOD,__LINE__)>::Put_internal(CALLABLE)
#define GET(PATH,CALLABLE) static const char COMBINE(METHOD,__LINE__)[]{ PATH };METHOD<COMBINE(METHOD,__LINE__)>::Get_internal(CALLABLE)
#define POST(PATH,CALLABLE) static const char COMBINE(METHOD,__LINE__)[]{ PATH };METHOD<COMBINE(METHOD,__LINE__)>::Post_internal(CALLABLE)
#define HEAD(PATH,CALLABLE) static const char COMBINE(METHOD,__LINE__)[]{ PATH };METHOD<COMBINE(METHOD,__LINE__)>::Head_internal(CALLABLE)

namespace GenericBoson
{
	template<const char* PATH>
	class METHOD
	{
		static void ThrowAlreadyRegisterdException(const std::string_view methodName)
		{
			std::stringstream strstream;
			strstream << "A " << methodName.data() << " method has already been registered at this " << PATH << "." << std::endl;
			throw new std::exception(strstream.str().c_str());
		}
	public:
		static std::function<void()> g_put_internal;
		static std::function<void()> g_get_internal;
		static std::function<void()> g_post_internal;
		static std::function<void()> g_head_internal;

		template<typename CALLABLE>
		static void Put_internal(CALLABLE callable)
		{
			if (nullptr != g_put_internal)
			{
				ThrowAlreadyRegisterdException("PUT");
			}

			g_put_internal = callable;
		}

		template<typename CALLABLE>
		static void Get_internal(CALLABLE callable)
		{
			if (nullptr != g_get_internal)
			{
				ThrowAlreadyRegisterdException("GET");
			}

			g_get_internal = callable;
		}

		template<typename CALLABLE>
		static void Post_internal(CALLABLE callable)
		{
			if (nullptr != g_post_internal)
			{
				ThrowAlreadyRegisterdException("POST");
			}

			g_post_internal = callable;
		}

		template<typename CALLABLE>
		static void Head_internal(CALLABLE callable)
		{
			if (nullptr != g_head_internal)
			{
				ThrowAlreadyRegisterdException("HEAD");
			}

			g_head_internal = callable;
		}
	};

	template<const char* PATH>
	std::function<void()> METHOD<PATH>::g_put_internal;
	template<const char* PATH>
	std::function<void()> METHOD<PATH>::g_get_internal;
	template<const char* PATH>
	std::function<void()> METHOD<PATH>::g_post_internal;
	template<const char* PATH>
	std::function<void()> METHOD<PATH>::g_head_internal;
}