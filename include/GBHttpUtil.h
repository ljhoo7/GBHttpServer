#pragma once

namespace GenericBoson
{
	// \param urlCandidate
	// \param parsedPath
	// \param queryMap
	// \return
	bool ParseUrlString(const std::string_view urlCandidate, std::vector<std::string_view>& parsedPath, std::map<std::string_view, std::string_view>&queryMap);

	// targetStringView가 가리키는 문자열을 좌측부터 우측으로 탐색해나가면서 가장 먼저 만나는 separator를 기준으로 양쪽에 있는 문자열들을 각각 가리키는 string_view를 output으로 내놓습니다.
	// \param targetStringView
	// \param separator
	// \param outputStringView
	// \return
	std::string_view Split(const std::string_view targetStringView, char separator, std::string_view& outputStringView);

	// targetStringView가 가리키는 문자열을 좌측부터 우측으로 탐색해나가면서 만나는 separator들로 targetStringView를 쪼개고 마지막으로 남는 문자열을 빼고는 모두 ref 벡터에 담아주고 마지막 문자열을 string_view로 반환해줍니다.
	// \param targetStringView
	// \param separator
	// \param outputArray
	// \return
	std::string_view Split(const std::string_view targetStringView, char separator, std::vector<std::string_view>& outputArray);
}