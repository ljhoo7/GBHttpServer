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
		bool Route(const std::string_view subStr);
	};
}