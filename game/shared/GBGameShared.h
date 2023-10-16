#pragma once

#include "StubAdaptor.h"

#include <format>
#include <memory>
#include <unordered_map>

namespace GenericBoson
{
	class GBGameShared
	{
	public:
		template<typename FLATBUFFER_TABLE>
		bool AddStub(const int messageID, void(*Stub)(const FLATBUFFER_TABLE& table))
		{
			const auto [_, isInserted] = m_stubs.emplace(messageID,
				std::make_shared<StubAdaptor<FLATBUFFER_TABLE>>(Stub));

			if (!isInserted)
			{
				throw std::format("Add stub failed. Message ID - {}", messageID);
			}
		}
	protected:
		std::unordered_map<int, std::shared_ptr<IStubAdaptor>> m_stubs;
	};
}