#pragma once

#include <memory>

namespace GenericBoson
{
	class ExpandedOverlapped;
	class Intersection
	{
	public:
		class Concept
		{
		public:
			virtual void SendPing(ExpandedOverlapped* pEol) = 0;
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

			void SendPing(ExpandedOverlapped* pEol) override { m_service.m_sendStrategy(pEol); }
		private:
			SERVICE m_service;
			SEND_STRATEGY m_sendStrategy;
		};

		template<typename SERVICE, typename SEND_STRATEGY>
		Intersection(SERVICE service, SEND_STRATEGY sendStrategy)
		{
			m_pImpl = std::make_unique<Model<SERVICE>>(
				std::move(service),
				std::move(sendStrategy)
			);
		}

	private:
		friend void SendPing(const Intersection& shared, ExpandedOverlapped* pEol)
		{
			shared.m_pImpl->SendPing(pEol);
		}

	private:
		std::unique_ptr<Concept> m_pImpl;
	};
}