#pragma once

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
	};
}