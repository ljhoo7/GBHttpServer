#pragma once

#include <map>
#include <variant>
#include <memory>
#include <cassert>

#include "../include/GBGETMethod.h"
#include "../include/GBHEADMethod.h"
#include "../include/GBPOSTMethod.h"
#include "GBHttpVersionTypes.h"

namespace GenericBoson
{
	class PathSegment
	{
		GET* m_pGetMethod = nullptr;
		HEAD* m_pHeadMethod = nullptr;
		POST* m_pPostMethod = nullptr;

		// #ToDo
		// PUT*
		// DELETE*
		// OPTIONS*
		// TRACE*
	public:
		std::map<std::string, std::shared_ptr<PathSegment>> m_subTreeMap;
	};
}