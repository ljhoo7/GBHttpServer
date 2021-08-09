#include "../include/stdafx.h"

#include "../include/GBHttpUtil.h"

namespace GenericBoson
{
	bool ParseUrlString(const std::string_view urlCandidate, std::vector<std::string_view>& parsedPath, std::map<std::string_view, std::string_view>& queryMap)
	{
		// 아래 if는 원래 false == urlCandidate.starts_with('/') 이었지만,
		// travis windows가 VS2017까지만 지원해서 아래처럼 바꿈.
		if (true == urlCandidate.empty() || '/' != urlCandidate.at(0))
		{
			return false;
		}

		size_t offset = 0;
		std::string_view urlCandidateCopy = urlCandidate.substr(1);

		std::string_view leftStringView1 = Split(urlCandidateCopy, '/', parsedPath);

		if (0 < leftStringView1.size())
		{
			offset = leftStringView1.find_first_of('?', 0);

			if (std::string_view::npos == offset)
			{
				// 쿼리가 없으므로 '/' 뒤에 몽땅 마지막 경로 조각으로 넣는다.
				parsedPath.emplace_back(leftStringView1);
			}
			else
			{
				// 쿼리가 있으므로 '/'와 '?' 사이의 것만 마지막 경로로 넣는다.
				std::string_view parsedSegment = leftStringView1.substr(0, offset);

				parsedPath.emplace_back(parsedSegment);

				leftStringView1 = leftStringView1.substr(offset + 1);

				std::vector<std::string_view> queryPairStringArray;
				std::string_view leftStringView2 = Split(leftStringView1, '&', queryPairStringArray);

				for(const auto& iQueryPair : queryPairStringArray)
				{

				}
			}
		}

		return true;
	}

	std::string_view Split(const std::string_view targetStringView, char separator, std::string_view* pOutputStringViewArray, int stringViewArraySize)
	{
		size_t offset = 0;
		std::string_view stringViewCopy = targetStringView;

		int cnt = 0;
		while (true)
		{
			offset = stringViewCopy.find_first_of('/', 0);

			if (std::string_view::npos == offset)
			{
				return stringViewCopy;
			}

			std::string_view parsedSegment = stringViewCopy.substr(0, offset);

			*pOutputStringViewArray = parsedSegment;

			pOutputStringViewArray++;
			cnt++;

			assert(cnt < stringViewArraySize);

			stringViewCopy = stringViewCopy.substr(offset + 1);
		}
	}

	std::string_view Split(const std::string_view targetStringView, char separator, std::vector<std::string_view>& outputArray)
	{
		return Split(targetStringView, separator, &outputArray[0], outputArray.size());
	}
}