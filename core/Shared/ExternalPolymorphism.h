#prgma once

namespace GenericBoson
{
	class Intersection
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
		PrivateShared(SERVICE service, SEND_STRATEGY sendStrategy)
		{
			m_pImpl = std::make_unique<Model<SERVICE>>(
				std::move(service),
				std::move(sendStrategy)
			);
		}

	private:
		friend void Send(const PrivateShared& shared)
		{
			shared.m_pImpl->Send();
		}

	private:
		std::unique_ptr<Concept> m_pImpl;
	};
}