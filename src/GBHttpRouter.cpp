#include "../include/stdafx.h"
#include "../include/GBHttpRouter.h"

namespace GenericBoson
{
	bool GBHttpRouterBase::Route(const PathSegment& rootPath, const std::string_view targetPath)
	{
		std::vector<std::string> pathSegmentArray;
		bool parseResult = ParseUrlString(targetPath, pathSegmentArray);

		if (false == parseResult)
		{
			return false;
		}



		return true;
	}
}