#pragma once

#include "../core/GBServer.h"
#include "StubAdaptor.h"
#include "BufferAllocator.h"

#include "flatbuffers/flatbuffers.h"
#include "winsock2.h"

#include <format>
#include <unordered_map>
#include <functional>

namespace GenericBoson
{
	struct GBExpandedOverlapped;

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

		template<typename CALLABLE>
		bool Send(GBExpandedOverlapped* pEol, const int messageID,
			CALLABLE&& callable)
		{
			if (!pEol)
			{
				// #ToDo
				ErrorLog("");
				return false;
			}

			::flatbuffers::FlatBufferBuilder fbb((size_t)BUFFER_SIZE, &g_bufferAllocator);

			fbb.Finish(std::forward<CALLABLE>(callable)(fbb));

			size_t size, offset;
			pEol->m_scatterOutput.m_pBuffer = reinterpret_cast<char*>(fbb.ReleaseRaw(size, offset));
			pEol->m_scatterOutput.m_pBuffer += offset;
			pEol->m_scatterOutput.m_offset = size;

			__super::Send(pEol);

			return true;
		};

		void SetConnectedTask(const std::function<void(GBExpandedOverlapped* pEol)>& task);

	private:

		bool Gather(GBExpandedOverlapped* pEol, const DWORD transferredBytes);

		virtual bool OnReceived(GBExpandedOverlapped* pEol, const DWORD transferredBytes) override;
		virtual bool OnSent(GBExpandedOverlapped* pEol, const DWORD transferredBytes) override;

		virtual bool ErrorLog(const std::string_view msg) override;
		virtual bool WarningLog(const std::string_view msg) override;
		virtual bool InfoLog(const std::string_view msg) override;

		virtual void OnConnected(GBExpandedOverlapped* pEol) override;

	private:
		const int MESSAGE_ID_SIZE = 2;
		const int LENGTH_SIZE = 2;

		std::function<void(GBExpandedOverlapped* pEol)> m_connectedTask;
		std::unordered_map<int, std::shared_ptr<IStubAdaptor>> m_stubs;

		static ThreadSafeBufferAllocator g_bufferAllocator;
	};
}