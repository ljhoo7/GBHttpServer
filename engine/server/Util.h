#pragma once

#include <string>
#include <vector>
#include <atomic>
#include <limits>

namespace GenericBoson
{
	std::string GetWSALastErrorString();
	std::string GetWSALastErrorString(int lastError);
}