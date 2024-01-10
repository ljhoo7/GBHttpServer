#include <chrono>
#include <utility>

namespace GenericBoson
{
	struct StopWatch
	{
		template<typename CALLABLE>
		static int64_t MeasureMs(CALLABLE&& callable)
		{
			const auto startTime = std::chrono::system_clock::now();
			std::forward<CALLABLE>(callable)();
			return std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::system_clock::now() - startTime).count();
		}
	};
}