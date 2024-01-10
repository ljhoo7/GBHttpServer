#include "stdafx.h"

#include "Util.h"
#include "Winsock2.h"

namespace GenericBoson
{
	std::string GetWSALastErrorString()
	{
		return GetWSALastErrorString(WSAGetLastError());
	}

	std::string GetWSALastErrorString(int lastError)
	{
		char* s = NULL;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, lastError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&s, 0, NULL);
		std::string errorString(s);
		LocalFree(s);

		return errorString;
	}
}