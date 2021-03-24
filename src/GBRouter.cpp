#include "../include/stdafx.h"
#include "../include/GBMethod.h"
#include "../include/GBRouter.h"

namespace GenericBoson
{
	std::string_view GBRouter::Route(const std::string_view subStr)
	{
		// method
		for (int i = 0; i < subStr.length(); ++i)
		{
			for (auto& iMethod : m_methodList)
			{
				assert(0 < iMethod.m_methodName.length());

				if (subStr[i] != iMethod.m_methodName[0])
				{
					continue;
				}

				if (subStr.length() <= i + iMethod.m_methodName.length())
				{
					continue;
				}

				if (iMethod.m_methodName == subStr.substr(i, iMethod.m_methodName.length()))
				{
					size_t token = subStr.find_first_of(' ', 4);
					return subStr.substr(4, token - 4);
				}
			}
		}

		return nullptr;
	}
}