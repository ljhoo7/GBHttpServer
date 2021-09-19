#include "../include/stdafx.h"
#include "../include/GBHttpStaticRouter.h"

namespace GenericBoson
{
	template<const char* PATH>
	std::function<void()> METHOD<PATH>::g_put_internal;
	template<const char* PATH>
	std::function<void()> METHOD<PATH>::g_get_internal;
	template<const char* PATH>
	std::function<void()> METHOD<PATH>::g_post_internal;
	template<const char* PATH>
	std::function<void()> METHOD<PATH>::g_head_internal;
}