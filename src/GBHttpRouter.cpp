#include "../include/stdafx.h"
#include "../include/GBHttpRouter.h"

namespace GenericBoson
{
	bool GBHttpRouterBase::Route(PathSegment& rootPath, const std::string_view targetPath, const std::string_view methodName)
	{
		std::vector<std::string> pathSegmentArray;
		bool parseResult = ParseUrlString(targetPath, pathSegmentArray);

		if (false == parseResult)
		{
			// #ToDo
			// The path in request line does not start with '/'.
			return false;
		}

		if (false == m_supportMethodMap.contains(methodName.data()))
		{
			// #ToDo
			// Invalid request : The method name is not matched with the version.
			return false;
		}

		PathSegment* pNode = &rootPath;
		for (auto& iPathSegment : pathSegmentArray)
		{
			if (false == pNode->m_subTreeMap.contains(iPathSegment))
			{
				// #ToDo ActionMethod not found.
				return false;
			}

			pNode = pNode->m_subTreeMap[iPathSegment].get();
		}

		if ("GET" == methodName)
		{
			if (nullptr == pNode->m_pGetMethod)
			{
				// #ToDo
				// The method not exist.
				return false;
			}

			pNode->m_pGetMethod->m_method(0);

			return true;
		}
		else if ("HEAD" == methodName)
		{
			if (nullptr == pNode->m_pHeadMethod)
			{
				// #ToDo
				// The method not exist.
				return false;
			}

			pNode->m_pHeadMethod->m_method(0);

			return true;
		}
		else if ("POST" == methodName)
		{
			if (nullptr == pNode->m_pPostMethod)
			{
				// #ToDo
				// The method not exist.
				return false;
			}

			pNode->m_pPostMethod->m_method(0);

			return true;
		}

		// #ToDo
		// Internal logic error
		return false;
	}
}