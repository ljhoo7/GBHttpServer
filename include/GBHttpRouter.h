#pragma once

#include <map>
#include <vector>
#include <memory>

#include "GBHttpResponse.h"
#include "GBPathSegment.h"
#include "GBHttpVersionTypes.h"
#include "GBHttpRequestLineReader.h"
#include "GBHttpUtil.h"
#include "GBMethod.h"

namespace GenericBoson
{
	class GBHttpRouterBase
	{
	public:
		std::map<std::string, std::shared_ptr<GBMethod>> m_supportMethodMap;

		GBHttpRouterBase() = default;
		virtual ~GBHttpRouterBase() = default;
		bool Route(PathSegment& rootPath, const std::string_view targetPath, const std::string_view methodName);
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
		GBHttpRouter()
		{
			auto pGet = std::make_shared<GBMethodGET>();

			m_supportMethodMap.emplace(pGet->GetName(), pGet);
		}

		virtual ~GBHttpRouter() = default;
	};

	template<>
	class GBHttpRouter<GBHttp10> : public GBHttpRouterBase
	{
	public:
		GBHttpRouter()
		{
			auto pGet = std::make_shared<GBMethodGET>();
			auto pHead = std::make_shared<GBMethodHEAD>();
			auto pPost = std::make_shared<GBMethodPOST>();

			m_supportMethodMap.emplace(pGet->GetName(), pGet);
			m_supportMethodMap.emplace(pHead->GetName(), pHead);
			m_supportMethodMap.emplace(pPost->GetName(), pPost);
		}
		virtual ~GBHttpRouter() = default;
	};

	template<>
	class GBHttpRouter<GBHttp11> : public GBHttpRouterBase
	{
	public:
		GBHttpRouter()
		{
			auto pGet = std::make_shared<GBMethodGET>();
			auto pHead = std::make_shared<GBMethodHEAD>();
			auto pPost = std::make_shared<GBMethodPOST>();

			m_supportMethodMap.emplace(pGet->GetName(), pGet);
			m_supportMethodMap.emplace(pHead->GetName(), pHead);
			m_supportMethodMap.emplace(pPost->GetName(), pPost);
		}
		virtual ~GBHttpRouter() = default;
	};
}