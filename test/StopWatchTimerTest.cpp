#include <gtest/gtest.h>

#include "../core/Shared/StopWatch.h"

#include <memory>
#include <chrono>

namespace GenericBoson
{
	struct StopWatchTest : public ::testing::Test
	{
		virtual void SetUp() override {}
		static void SetUpTestCase() {}
		static void TearDownTestCase() {}
	};

	TEST_F(StopWatchTest, BasicTest) {
		const auto elapsedTimeMs = StopWatch::MeasureMs([]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			});
		EXPECT_TRUE(930 <= elapsedTimeMs && elapsedTimeMs <= 1030);
	}
}

