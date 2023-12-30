#pragma once

#include <string_view>
#include <format>
#include <cassert>

namespace GenericBoson
{
	typedef size_t BUFFER_SIZE_TYPE;
	constexpr BUFFER_SIZE_TYPE BUFFER_SIZE = 8 * 1024 + 200;

	enum class IO_TYPE : int32_t
	{
		ACCEPT = 1,
		RECEIVE,
		SEND,
	};

	class VectoredIO
	{
	public:
		enum STATE : char
		{
			ID = 0,
			LENGTH,
			PAYLOAD,
			MAX_STATE
		};
	public:
		void AdvanceState()
		{
			m_state = STATE{ (char{ m_state } + 1) % STATE::MAX_STATE };
		}

		STATE GetState() const
		{
			return m_state;
		}

	public:
		char m_buffer[BUFFER_SIZE] = { 1, };

		int32_t m_messageID = 0;
		int32_t m_length = 0, m_offset = 0;

	private:
		STATE m_state = STATE::ID;
	};

	class Shared
	{
	public:
		class Concept
		{
		public:
			virtual void Send() = 0;
		};

		template<typename SERVICE, typename SEND_STRATEGY>
		class Model : public Concept
		{
		public:
			explicit Model(SERVICE service, 
				SEND_STRATEGY sendStrategy)
				: m_service{ std::move(service) }
				, m_sendStrategy{ std::move(sendStrategy) }
			{}

			void Send() override { m_sendStrategy(m_service); }
		private:
			SERVICE m_service;
			SEND_STRATEGY m_sendStrategy;
		};

		template<typename SERVICE, typename SEND_STRATEGY>
		Shared(SERVICE service, SEND_STRATEGY sendStrategy)
		{
			m_pImpl = std::make_unique<Model<SERVICE>>(
				std::move(service),
				std::move(sendStrategy)
			);
		}

		virtual bool ErrorLog(const std::string_view msg);
		virtual bool WarningLog(const std::string_view msg);
		virtual bool InfoLog(const std::string_view msg);

		virtual bool OnGatheringCompleted(VectoredIO& inputData) { return false; };
	
		bool ReadWholePartialMessages(VectoredIO& inputData, const unsigned long transferredBytes);
		bool OnReceived(VectoredIO& inputData, const unsigned long transferredBytes);
		bool OnSent(VectoredIO& outputData, const unsigned long transferredBytes);
	private:
		bool Gather(VectoredIO& vectoredIO, const unsigned long transferredBytes);

		friend void Send(const Shared& shared)
		{
			shared.m_pImpl->Send();
		}

	private:
		std::unique_ptr<Concept> m_pImpl;
	};
}