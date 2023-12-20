#pragma once

#include "../../core/server/Server.h"
#include "BufferAllocator.h"

#include "flatbuffers/flatbuffers.h"

#include "../shared/GameShared.h"
#include "../shared/Constant.h"

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
			static_assert(messageID < ENGINE_RESERVED_PROTOCOL_NUMBER_RANGE_START);
			static_assert(ENGINE_RESERVED_PROTOCOL_NUMBER_RANGE_END < messageID);

			return m_GameShared.AddStubInternal(messageID, Stub);
		}

		template<typename CALLABLE>
		bool Send(GBExpandedOverlapped* pEol, const int32_t messageID,
			CALLABLE&& callable)
		{
			if (!pEol)
			{
				// #ToDo
				m_GameShared.ErrorLog("");
				return false;
			}

			::flatbuffers::FlatBufferBuilder fbb((size_t)BUFFER_SIZE, &g_bufferAllocator);

			// Do not use FinishSizePrefixed because it makes buffer invalid.
			fbb.Finish(std::forward<CALLABLE>(callable)(fbb));

			size_t size, offset;
			char* pFlatRawBuffer = reinterpret_cast<char*>(fbb.ReleaseRaw(size, offset));

			int32_t tableSize = size - offset;

			char* buffer = pEol->m_outputData.m_buffer;

			memcpy_s(buffer, BUFFER_SIZE, &messageID, sizeof(messageID));
			buffer += sizeof(messageID);
			memcpy_s(buffer, BUFFER_SIZE, &tableSize, sizeof(tableSize));
			buffer += sizeof(tableSize);
			memcpy_s(buffer, BUFFER_SIZE, pFlatRawBuffer + offset, tableSize);
			buffer += tableSize;

			pEol->m_outputData.m_offset = tableSize + sizeof(messageID) + sizeof(tableSize);

			__super::Send(pEol);

			return true;
		};

		void SetConnectedTask(const std::function<void(GBExpandedOverlapped* pEol)>& task);

	private:
		virtual void OnConnected(GBExpandedOverlapped* pEol) override;
	private:
		GBGameShared m_GameShared;

		const int MESSAGE_ID_SIZE = 2;
		const int LENGTH_SIZE = 2;

		std::function<void(GBExpandedOverlapped* pEol)> m_connectedTask;
		
		static ThreadSafeBufferAllocator g_bufferAllocator;
	};
}