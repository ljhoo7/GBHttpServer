#include "../include/stdafx.h"

#include "../include/GBHttpUtil.h"

namespace GenericBoson
{
	bool ParseUrlString(const std::string_view urlCandidate, std::vector<std::string>& parsedPath)
	{
		if (false == urlCandidate.starts_with('/'))
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
					parsedPath.emplace_back(urlCandidateCopy);
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