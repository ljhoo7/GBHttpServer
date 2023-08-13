#include "GBHttpUtil.h"

namespace GenericBoson
{
	bool ParseUrlString(const std::string_view urlCandidate, std::vector<std::string_view>& parsedPath, std::map<std::string_view, std::string_view>& queryMap, std::string_view fragment)
	{
		// 아래 if는 원래 false == urlCandidate.starts_with('/') 이었지만,
		// travis windows가 VS2017까지만 지원해서 아래처럼 바꿈.
		if (true == urlCandidate.empty() || '/' != urlCandidate.at(0))
		{
			return false;
		}

		size_t offset = 0;
		std::string_view urlCandidateCopy = urlCandidate.substr(1);

		std::string_view queryAndFragment = Split(urlCandidateCopy, '/', '?', parsedPath);

		if (true == queryAndFragment.empty())
		{
			return true;
		}

		std::vector<std::string_view> queryPairStringArray;
		fragment = Split(queryAndFragment, '&', '#', queryPairStringArray);

		for (const auto& iQueryPairStr : queryPairStringArray)
		{
			std::string_view valueStr;
			std::string_view keyStr = Split(iQueryPairStr, '=', valueStr);
			queryMap.emplace(keyStr, valueStr);
		}

		return true;
	}

	std::string_view Split(const std::string_view targetStringView, char separator, std::string_view& outputStringView)
	{
		size_t offset = targetStringView.find_first_of(separator, 0);

		if (std::string_view::npos == offset)
		{
			return targetStringView;
		}

		outputStringView = targetStringView.substr(offset + 1);

		return targetStringView.substr(0, offset);
	}

	std::string_view Split(const std::string_view targetStringView, char separator, std::vector<std::string_view>& outputArray)
	{
		std::string_view stringViewCopy = targetStringView;

		while (true)
		{
			std::string_view leftPart;
			std::string_view parsedPart = Split(stringViewCopy, separator, leftPart);

			if (true == leftPart.empty())
			{
				return parsedPart;
			}

			stringViewCopy = leftPart;
			outputArray.push_back(parsedPart);
		}
	}

	std::string_view Split(const std::string_view targetStringView, char separator, char endCharacter, std::vector<std::string_view>& outputArray)
	{
		std::string_view leftString = Split(targetStringView, separator, outputArray);

		if (0 == leftString.size())
		{
			return {};
		}

		size_t offset = leftString.find_first_of(endCharacter, 0);

		if (std::string_view::npos != offset)
		{
			// endCharacter가 있으므로 separator와 endCharacter 사이의 것만 outputArray에 넣는다.
			std::string_view parsedSegment = leftString.substr(0, offset);

			outputArray.emplace_back(parsedSegment);

			return leftString.substr(offset + 1);
		}

		// endCharacter가 없으므로 separator 뒤에 몽땅 outputArray에 넣는다.
		outputArray.emplace_back(leftString);

		return {};
	}
}