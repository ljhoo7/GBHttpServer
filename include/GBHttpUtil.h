#pragma once

namespace GenericBoson
{
	bool ParseUrlString(const std::string_view urlCandidate, std::vector<std::string>& parsedPath, std::map<std::string, std::string>&queryMap);

	std::string_view Split(const std::string_view targetStringView, char separator, std::vector<std::string>& outputArray);
}