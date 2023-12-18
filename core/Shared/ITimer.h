#pragma once

namespace GenericBoson
{
	class ITimer
	{
	public:
		ITimer(const int64_t periodNs)
			: m_PeriodNs(periodNs)
		{}

		virtual ~ITimer() = default;

		virtual void OnTime() = 0;

		int64_t PeriodMs() const
		{
			return m_PeriodNs;
		}
	private:
		int64_t m_PeriodNs;
	};

	struct TimerComparer
	{
		bool operator()(const std::shared_ptr<ITimer>& lhs, const std::shared_ptr<ITimer>& rhs) { return lhs->PeriodMs() < rhs->PeriodMs(); };
	};
}