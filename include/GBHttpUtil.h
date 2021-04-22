#pragma once

namespace GenericBoson
{
	bool ParseUrlString(const std::string_view urlCandidate, std::vector<std::string>& parsedPath);
}