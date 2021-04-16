#pragma once

#include <map>
#include <variant>
#include <memory>

#include "../include/GBMethod.h"

namespace GenericBoson
{
	template<typename ROUTER>
	class PathSegment
	{
	public:
		std::map<std::string, std::shared_ptr<PathSegment>> m_subTreeMap;

		/*
		key - method name ( small letter only ).
		value - GBMethod pointer.
		*/
		std::map<std::string, std::shared_ptr<GBMethod>> m_methodMap;
	};
}