#pragma once

#include <map>
#include <vector>

#include "../lib/GBString/include/GBString.h"
#include "GBResponse.h"
#include "GBPathSegment.h"
#include "GBHttpVersionTypes.h"
#include "GBHttpRequestLineReader.h"

namespace GenericBoson
{
	class GBHttpRouterBase
	{
		const SOCKET& m_acceptedSocket;
	public:
		GBHttpRouterBase(const SOCKET& acceptedSocket) : m_acceptedSocket(acceptedSocket) {}
		virtual ~GBHttpRouterBase() = default;
		bool Route(const std::map<std::string, PathSegment>& subStr)
		{
			return true;
		}
	};

	template<typename HTTPVERSION>
	class GBHttpRouter
	{
		GBHttpRouter() = delete;
	};

	template<>
	class GBHttpRouter<GBHttp09> : public GBHttpRouterBase
	{
	public:
		GBHttpRouter(const SOCKET& acceptedSocket) : GBHttpRouterBase(acceptedSocket) {}
		virtual ~GBHttpRouter() = default;
	};

	template<>
	class GBHttpRouter<GBHttp10> : public GBHttpRouterBase
	{
	public:
		GBHttpRouter(const SOCKET& acceptedSocket) : GBHttpRouterBase(acceptedSocket) {}
		virtual ~GBHttpRouter() = default;
	};

	template<>
	class GBHttpRouter<GBHttp11> : public GBHttpRouterBase
	{
	public:
		GBHttpRouter(const SOCKET& acceptedSocket) : GBHttpRouterBase(acceptedSocket) {}
		virtual ~GBHttpRouter() = default;
	};
}