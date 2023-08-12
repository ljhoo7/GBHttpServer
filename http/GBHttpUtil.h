#pragma once

namespace GenericBoson
{
	// \param urlCandidate
	// \param parsedPath
	// \param queryMap
	// \param fragment
	// \return
	bool ParseUrlString(const std::string_view urlCandidate, std::vector<std::string_view>& parsedPath, std::map<std::string_view, std::string_view>&queryMap, std::string_view fragment);

	// targetStringView가 가리키는 문자열을 좌측부터 우측으로 탐색해나가면서 가장 먼저 만나는 separator를 기준으로 양쪽에 있는 문자열들을 각각 가리키는 string_view를 output으로 내놓습니다.
	// \param targetStringView
	// \param separator
	// \param outputStringView
	// \return
	std::string_view Split(const std::string_view targetStringView, char separator, std::string_view& outputStringView);

	// targetStringView가 가리키는 문자열을 좌측부터 우측으로 탐색해나가면서 만나는 separator들로 targetStringView를 쪼개고 마지막으로 남는 문자열을 빼고는 모두 ref out 벡터에 담아주고 마지막 문자열을 string_view로 반환해줍니다.
	// \param targetStringView
	// \param separator
	// \param outputArray
	// \return
	std::string_view Split(const std::string_view targetStringView, char separator, std::vector<std::string_view>& outputArray);

	// targetStringView가 가리키는 문자열을 좌측부터 우측으로 탐색해나가면서 만나는 separator들로 targetStringView를 쪼개고 마지막으로 남는 문자열이 endCharacter로 나눌 수 있는지 본다.
	// 나눌 수 있다면, 제일 처음에 나온 endCharacter로 나누고 앞부분은 ref out 벡터에 넣고 나머지는 반환한다.
	// 나눌 수 없다면, 남은 문자열 모두 ref out 벡터에 담고, 빈 string_view를 반환한다.
	// \param targetStringView
	// \param separator
	// \param endCharacter
	// \param outputArray
	// \return
	std::string_view Split(const std::string_view targetStringView, char separator, char endCharacter, std::vector<std::string_view>& outputArray);
}