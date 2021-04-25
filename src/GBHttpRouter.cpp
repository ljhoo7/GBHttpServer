#include "../include/stdafx.h"
#include "../include/GBHttpRouter.h"

namespace GenericBoson
{
	bool GBHttpRouterBase::Route(const PathSegment& rootPath, const std::string_view targetPath, const std::string_view methodName)
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

		const PathSegment* pNode = &rootPath;
		for (auto& iPathSegment : pathSegmentArray)
		{
			if (false == rootPath.m_subTreeMap.contains(iPathSegment))
			{
				// #ToDo ActionMethod not found.
				return false;
			}
		}

		//pNode->m_

		return true;
	}
}