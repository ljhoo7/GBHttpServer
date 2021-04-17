#pragma once

#include <map>
#include <vector>

#include "../lib/GBString/include/GBString.h"
#include "GBResponse.h"
#include "GBPathSegment.h"
#include "GBHttpVersionTypes.h"

namespace GenericBoson
{
	struct SOCKET;

	class GBHttpRouterBase
	{
		const SOCKET& m_acceptedSocket;
	public:
		GBHttpRouterBase(const SOCKET& acceptedSocket) : m_acceptedSocket(acceptedSocket) {}
		virtual ~GBHttpRouterBase() = default;
		virtual bool Route(const GBStringView subStr) = 0;
	};

	template<typename HTTPVERSION>
	class GBHttpRouter
	{
		GBHttpRouter() = delete;
	};

	template<>
	class GBHttpRouter<GBHttp09> : public GBHttpRouterBase
	{
		std::map<std::string, PathSegment<GBHttp09>> m_methodTree;
	public:
		GBHttpRouter(const SOCKET& acceptedSocket) : GBHttpRouterBase(acceptedSocket) {}
		virtual ~GBHttpRouter() = default;
		virtual bool Route(const GBStringView subStr) override;
	};

	template<>
	class GBHttpRouter<GBHttp10> : public GBHttpRouterBase
	{
		std::map<std::string, PathSegment<GBHttp10>> m_methodTree;
	public:
		GBHttpRouter(const SOCKET& acceptedSocket) : GBHttpRouterBase(acceptedSocket) {}
		virtual ~GBHttpRouter() = default;
		virtual bool Route(const GBStringView subStr) override;
	};

	template<>
	class GBHttpRouter<GBHttp11> : public GBHttpRouterBase
	{
		std::map<std::string, PathSegment<GBHttp11>> m_methodTree;
	public:
		GBHttpRouter(const SOCKET& acceptedSocket) : GBHttpRouterBase(acceptedSocket) {}
		virtual ~GBHttpRouter() = default;
		virtual bool Route(const GBStringView subStr) override;
	};
}