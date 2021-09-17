#include "../include/stdafx.h"
#include "../include/GBHttpStaticRouter.h"

namespace GenericBoson
{
	template<const char* PATH>
	std::function<void()> METHOD<PATH>::g_put;
	template<const char* PATH>
	std::function<void()> METHOD<PATH>::g_get;
	template<const char* PATH>
	std::function<void()> METHOD<PATH>::g_post;
	template<const char* PATH>
	std::function<void()> METHOD<PATH>::g_head;
}