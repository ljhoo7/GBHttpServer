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

	class IMessageHandlerAdaptor
	{
	public:
		virtual std::pair<bool, ::flatbuffers::FlatBufferBuilder> CallReqHandler() = 0;
		virtual bool CallResHandler(char* rawBuffer) = 0;
	};

	template<typename REQUEST_HANDLER, typename RESPONSE_HANDLER>
	class MessageHandlerAdaptor : public IMessageHandlerAdaptor
	{
	public:
		MessageHandlerAdaptor(const REQUEST_HANDLER& reqHandler, const RESPONSE_HANDLER& resHandler)
			: m_reqHandler(reqHandler), m_resHandler(resHandler) {}

		virtual std::pair<bool, ::flatbuffers::FlatBufferBuilder> CallReqHandler() override
		{
			return m_reqHandler();
		}

		virtual bool CallResHandler(char* rawBuffer) override
		{
			return m_resHandler(rawBuffer);
		}

	public:
		const REQUEST_HANDLER m_reqHandler;
		const RESPONSE_HANDLER m_resHandler;
	};

	class GBGameServer : public GBServer
	{
	public:
		GBGameServer(uint16_t portNum) : GBServer(portNum) {}
		virtual ~GBGameServer() = default;

		template<typename REQUEST_HANDLER, typename RESPONSE_HANDLER>
		bool AddHandler(const int messageID, const REQUEST_HANDLER& requestHandler, const RESPONSE_HANDLER& responseHandler)
		{
			const auto [_, isInserted] = m_handlers.emplace(messageID,
				std::make_shared<MessageHandlerAdaptor<REQUEST_HANDLER, RESPONSE_HANDLER>>(requestHandler, responseHandler));

			if (!isInserted)
			{
				throw std::format("Add handler failed. Message ID - {}", messageID);
			}
		}
	private:
		bool Send(const int messageID);

		bool Gather(GBExpandedOverlapped* pEol, const DWORD transferredBytes)
		{
			if (pEol->m_gatherInput.m_length < pEol->m_gatherInput.m_offset + transferredBytes)
			{
				return false;
			}

			pEol->m_gatherInput.m_offset = 0;
			pEol->m_gatherInput.AdvanceState();

			return true;
		}

		virtual bool OnReceived(GBExpandedOverlapped* pEol, const DWORD transferredBytes) override;
		virtual bool OnSent(GBExpandedOverlapped* pEol, const DWORD transferredBytes) override;

		virtual bool ErrorLog(const std::string_view msg) override;
		virtual bool WarningLog(const std::string_view msg) override;
		virtual bool InfoLog(const std::string_view msg) override;

	private:
		const int MESSAGE_ID_SIZE = 2;
		const int LENGTH_SIZE = 2;

		std::unordered_map<int, std::shared_ptr<IMessageHandlerAdaptor>> m_handlers;
	};
}