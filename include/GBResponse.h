#pragma once

#include <string_view>

#include "../lib/GBString/include/GBString.h"

namespace GenericBoson
{
	bool SendResponse(const SOCKET& acceptedSocket, const GBStringView path);
}