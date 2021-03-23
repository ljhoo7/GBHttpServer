#include "../include/stdafx.h"
#include "../include/GBRouter.h"

namespace GenericBoson
{
	std::string_view GBRouter::Route(const std::string_view subStr)
	{
		// method
		for (int i = 0; i < subStr.length(); ++i)
		{
			for (auto iMethod : m_methodList)
			{
				size_t iMethodSize = m_methodNameSizeMap[iMethod];
				assert(0 < iMethodSize);

				if (subStr[i] != iMethod[0])
				{
					continue;
				}

				if (subStr.length() <= i + iMethodSize)
				{
					continue;
				}

				if (iMethod == subStr.substr(i, iMethodSize))
				{
					size_t token = subStr.find_first_of(' ', 4);
					return subStr.substr(4, token - 4);
				}
			}
		}

		return nullptr;
	}
}