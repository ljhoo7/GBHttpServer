#include <chrono>

namespace GenericBoson
{
	struct StopWatch
	{
		template<typename CALLABLE>
		int64_t MeasureMs(const CALLABLE&& callable)
		{
			const auto startTime = std::chrono::system_clock::now();
			std::forward(callable)();
			return std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::system_clock::now() - startTime).count();
		}
	};
}