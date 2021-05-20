#pragma once

namespace GenericBoson
{
	class GBHttpVersion {};
	class GBHttp09 : public GBHttpVersion {};
	class GBHttp10 : public GBHttpVersion {};
	class GBHttp11 : public GBHttpVersion {};

	enum class HttpVersion
	{
		None,
		StillLeftToReceive,
		Http09,
		Http10,
		Http11,
	};
}