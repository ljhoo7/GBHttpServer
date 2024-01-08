#pragma once

#include "flatbuffers/flatbuffers.h"

namespace GenericBoson
{
	class IStubAdaptor
	{
	public:
		virtual void CallStub(char* rawBuffer) = 0;
	};

	template<typename FLATBUFFER_TABLE>
	class StubAdaptor : public IStubAdaptor
	{
	public:
		StubAdaptor(void(*Stub)(const FLATBUFFER_TABLE& table))
			: m_Stub(Stub) {}

		virtual void CallStub(char* rawBuffer) override
		{
			auto pTable = ::flatbuffers::GetRoot<FLATBUFFER_TABLE>(rawBuffer);
			m_Stub(*pTable);
		}

	public:
		void(*m_Stub)(const FLATBUFFER_TABLE& table);
	};
}