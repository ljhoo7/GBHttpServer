#pragma once

namespace GenericBoson
{
	class GBHttpVersion {};
	class GBHttp09 : public GBHttpVersion {};
	class GBHttp10 : public GBHttpVersion {};
	class GBHttp11 : public GBHttpVersion {};

	enum class HttpVersion : int
	{
		None = 0,
		Http09 = 9,
		Http10 = 10,
		Http11 = 11,
	};
}