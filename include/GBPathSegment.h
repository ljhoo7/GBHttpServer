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
	class PathSegment;

	typedef std::map<std::string_view, std::unique_ptr<PathSegment>> GBHttpPathTreeNode;

	struct PathSegment
	{
		std::unique_ptr<GBMethodGET> m_pGetMethod = nullptr;
		std::unique_ptr<GBMethodHEAD> m_pHeadMethod = nullptr;
		std::unique_ptr<GBMethodPOST> m_pPostMethod = nullptr;

		// #ToDo
		// PUT*
		// DELETE*
		// OPTIONS*
		// TRACE*

		GBHttpPathTreeNode m_subTreeMap;
	};
}