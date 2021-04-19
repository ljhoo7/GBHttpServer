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
	template<typename HTTPVERSION>
	class PathSegment
	{
		PathSegment() = delete;
	};

	template<>
	class PathSegment<GBHttp09>
	{
		GBHttp09 test;
	public:
		std::map<std::string, std::shared_ptr<PathSegment<GBHttp09>>> m_subTreeMap;
	};

	template<>
	class PathSegment<GBHttp10>
	{
		GET* m_pGetMethod = nullptr;
		HEAD* m_pHeadMethod = nullptr;
		POST* m_pPostMethod = nullptr;
	public:
		std::map<std::string, std::shared_ptr<PathSegment<GBHttp10>>> m_subTreeMap;
	};

	template<>
	class PathSegment<GBHttp11>
	{
		GET* m_pGetMethod = nullptr;

		// #ToDo
		// PUT*
		// DELETE*
		// OPTIONS*
		// TRACE*
	public:
		std::map<std::string, std::shared_ptr<PathSegment<GBHttp11>>> m_subTreeMap;
	};
}