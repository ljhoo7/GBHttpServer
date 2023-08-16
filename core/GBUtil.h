#pragma once

#include <string>

namespace GenericBoson
{
	std::string GetWSALastErrorString();
	std::string GetWSALastErrorString(int lastError);
}