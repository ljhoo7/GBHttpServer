#include "../include/stdafx.h"

#include "../include/GBHttpUtil.h"

namespace GenericBoson
{
	bool ParseUrlString(const std::string_view urlCandidate, std::vector<std::string>& parsedPath, std::map<std::string, std::string>& queryMap)
	{
		// 아래 if는 원래 false == urlCandidate.starts_with('/') 이었지만,
		// travis windows가 VS2017까지만 지원해서 아래처럼 바꿈.
		if (true == urlCandidate.empty() || '/' != urlCandidate.at(0))
		{
			return false;
		}

		size_t offset = 0;
		std::string_view urlCandidateCopy = urlCandidate.substr(1);

		while(true)
		{
			offset = urlCandidateCopy.find_first_of('/', 0);

			if (std::string_view::npos == offset)
			{
				if (0 < urlCandidate.size())
				{
					while(true)
					{
						offset = urlCandidateCopy.find_first_of('?', 0);

						if (std::string_view::npos == offset)
						{
							if (0 < urlCandidate.size())
							{
								parsedPath.emplace_back(urlCandidateCopy);
							}

							break;
						}

						std::string_view parsedSegment = urlCandidateCopy.substr(0, offset);

						parsedPath.emplace_back(parsedSegment);

						urlCandidateCopy = urlCandidateCopy.substr(offset + 1);
					}
				}

				break;
			}

			std::string_view parsedSegment = urlCandidateCopy.substr(0, offset);

			parsedPath.emplace_back(parsedSegment);

			urlCandidateCopy = urlCandidateCopy.substr(offset + 1);
		}

		return true;
	}
}