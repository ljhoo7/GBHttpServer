#include "../include/stdafx.h"
#include "../include/GBMethod.h"
#include "../include/GBResponse.h"
#include "../include/GBRouter.h"

namespace GenericBoson
{
	bool GBRouter::Route(const std::string_view subStr)
	{
		// method
		for (int i = 0; i < subStr.length(); ++i)
		{
			for (auto& iMethod : m_methodList)
			{
				assert(0 < iMethod.m_methodName.length());

				if (subStr.length() <= i + iMethod.m_methodName.length())
				{
					continue;
				}

				if (iMethod.m_methodName == subStr.substr(i, iMethod.m_methodName.length()))
				{
					size_t lengthToSkip = iMethod.m_methodName.length() + 1;
					size_t token = subStr.find_first_of(' ', lengthToSkip);

					std::string_view path = subStr.substr(lengthToSkip, token - lengthToSkip);

					iMethod.m_method(path);

					return GenericBoson::SendResponse(m_acceptedSocket, path);
				}
			}
		}

		return false;
	}
}