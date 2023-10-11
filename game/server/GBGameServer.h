#pragma once

#include "../../core/server/GBServer.h"
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