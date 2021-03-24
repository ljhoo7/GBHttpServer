#pragma once

#include <vector>

namespace GenericBoson
{
	class GBMethod;

	class GBRouter
	{
	public:
		std::vector<GBMethod> m_methodList;

		GBRouter() = default;
		std::string_view Route(const std::string_view subStr);
	};
}