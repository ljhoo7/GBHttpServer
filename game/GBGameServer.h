#pragma once

#include "../core/GBServer.h"
#include "flatbuffers/flatbuffers.h"
#include "winsock2.h"

#include <format>
#include <unordered_map>
#include <functional>

namespace GenericBoson
{
	struct GBExpandedOverlapped;

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

	class GBGameServer : public GBServer
	{
	public:
		GBGameServer(uint16_t portNum) : GBServer(portNum) {}
		virtual ~GBGameServer() = default;

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

		bool Send(GBExpandedOverlapped* pEol, const int messageID,
			::flatbuffers::FlatBufferBuilder& fbb);
		void SetConnectedTask(const std::function<void(GBExpandedOverlapped* pEol)>& task)
		{
			m_connectedTask = task;
		}

	private:

		bool Gather(GBExpandedOverlapped* pEol, const DWORD transferredBytes)
		{
			if (pEol->m_gatherInput.m_length < pEol->m_gatherInput.m_offset + transferredBytes)
			{
				return false;
			}

			pEol->m_gatherInput.m_offset = 0;
			pEol->m_gatherInput.AdvanceState();

		}

		virtual bool OnReceived(GBExpandedOverlapped* pEol, const DWORD transferredBytes) override;
		virtual bool OnSent(GBExpandedOverlapped* pEol, const DWORD transferredBytes) override;

		virtual bool ErrorLog(const std::string_view msg) override;
		virtual bool WarningLog(const std::string_view msg) override;
		virtual bool InfoLog(const std::string_view msg) override;

		virtual void OnConnected(GBExpandedOverlapped* pEol) override
		{
			if (m_connectedTask)
			{
				m_connectedTask(pEol);
			}
		}

	private:
		const int MESSAGE_ID_SIZE = 2;
		const int LENGTH_SIZE = 2;

		std::function<void(GBExpandedOverlapped* pEol)> m_connectedTask;

		std::unordered_map<int, std::shared_ptr<IStubAdaptor>> m_stubs;
	};
}