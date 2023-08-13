#pragma once

#include <tuple>
#include <functional>
#include <thread>
#include <mutex>


#include "GBHttpResponseWriter.h"
#include "GBHttpRequestReader.h"

namespace GenericBoson
{
	class GBHttpServer : public GBServer
	{
		////
		//// \param pathTree
		//// \param queryMap
		//// \param pTargetPath
		//bool TraversePathTree(const std::vector<std::string_view>& pathTree, const std::map<std::string_view, std::string_view> queryMap, PathSegment*& pTargetPath);

		// Equivalent to '/'
		/*static PathSegment g_rootPath;
		static std::unique_ptr<GBHttpRouterBase> g_pRouter;*/

		virtual bool OnReceived(GBExpandedOverlapped* pEol, DWORD receivedBytes) override;
		virtual bool OnSent(GBExpandedOverlapped* pEol, DWORD sentBytes) override;

	public:
		std::pair<bool, std::string> Start();
	};
}