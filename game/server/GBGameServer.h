#pragma once

#include "../../core/server/GBServer.h"
#include "BufferAllocator.h"

#include "flatbuffers/flatbuffers.h"

#include "../shared/GBGameShared.h"

#include <unordered_map>
#include <functional>

namespace GenericBoson
{
	struct GBExpandedOverlapped;

	class GBGameServer : public GBServer
	{
	public:
		GBGameServer(uint16_t portNum) : m_pShared{ std::make_unique<GBGameShared>() }, GBServer(portNum) {}
		virtual ~GBGameServer() = default;

		template<typename FLATBUFFER_TABLE>
		bool AddStub(const int messageID, void(*Stub)(const FLATBUFFER_TABLE& table))
		{
			return m_pShared->AddStubInternal(messageID, Stub);
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
			char* pFlatRawBuffer = reinterpret_cast<char*>(fbb.ReleaseRaw(size, offset));

			char* buffer = pEol->m_scatterOutput.m_buffer;

			memcpy_s(buffer, BUFFER_SIZE, &messageID, sizeof(messageID));
			buffer += sizeof(messageID);
			memcpy_s(buffer, BUFFER_SIZE, &size, sizeof(size));
			buffer += sizeof(size);
			memcpy_s(buffer, BUFFER_SIZE, pFlatRawBuffer, size);
			buffer += size;

			pEol->m_scatterOutput.m_offset = size + sizeof(messageID) + sizeof(size);

			__super::Send(pEol);

			return true;
		};

		void SetConnectedTask(const std::function<void(GBExpandedOverlapped* pEol)>& task);

	private:
		virtual bool ErrorLog(const std::string_view msg) override;
		virtual bool WarningLog(const std::string_view msg) override;
		virtual bool InfoLog(const std::string_view msg) override;

		virtual void OnConnected(GBExpandedOverlapped* pEol) override;
	private:
		const int MESSAGE_ID_SIZE = 2;
		const int LENGTH_SIZE = 2;

		std::function<void(GBExpandedOverlapped* pEol)> m_connectedTask;
		
		static ThreadSafeBufferAllocator g_bufferAllocator;
	};
}