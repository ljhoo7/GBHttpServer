#pragma once

namespace GenericBoson
{
	bool ParseUrlString(const std::string_view urlCandidate, std::vector<std::string_view>& parsedPath, std::map<std::string_view, std::string_view>&queryMap);

	std::string_view Split(const std::string_view targetStringView, char separator, std::string_view& outputStringView);

	std::string_view Split(const std::string_view targetStringView, char separator, std::vector<std::string_view>& outputArray);
}