#pragma once

#include <set>
#include <variant>

namespace GenericBoson
{
	//using METHOD = std::variant<GET,HEAD,POST>;

	class PathSegment
	{
		std::set<PathSegment> m_subPath;

		//std::vector<METHOD> m_methodArray;
	};
}