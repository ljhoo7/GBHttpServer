#pragma once

#include <string_view>

namespace GenericBoson
{
	bool SendResponse(const SOCKET& acceptedSocket, const std::string_view path);
}