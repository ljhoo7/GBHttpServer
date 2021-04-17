#pragma once

#include <map>
#include <variant>
#include <memory>

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
		GET* m_pGetMethod = nullptr;
	public:
		PathSegment() = default;
		std::map<std::string, std::shared_ptr<PathSegment<GBHttp09>>> m_subTreeMap;
	};

	template<>
	class PathSegment<GBHttp10>
	{
		GET* m_pGetMethod = nullptr;
		HEAD* m_pGetMethod = nullptr;
		POST* m_pGetMethod = nullptr;
	public:
		PathSegment() = default;
		std::map<std::string, std::shared_ptr<PathSegment<GBHttp09>>> m_subTreeMap;
	};

	template<>
	class PathSegment<GBHttp09>
	{
		GET* m_pGetMethod = nullptr;

		// #ToDo
		// PUT*
		// DELETE*
		// OPTIONS*
		// TRACE*
	public:
		PathSegment() = default;
		std::map<std::string, std::shared_ptr<PathSegment<GBHttp09>>> m_subTreeMap;
	};
}