#define BOOST_THREAD_VERSION 5

#include "../engine/Shared/StopWatch.h"
#include "boost/thread/future.hpp"

#include <gtest/gtest.h>

#include <shared_mutex>
#include <future>

namespace GenericBoson
{
	struct SharedMutexest : public ::testing::Test
	{
		virtual void SetUp() override {}
		static void SetUpTestCase() {}
		static void TearDownTestCase() {}
	};

	TEST(SharedMutexest, ReadLockOnlyTest) {
		const auto elapsedTimeMs = StopWatch::MeasureMs(
			[]()
			{
				std::shared_mutex sharedMutex;
				boost::when_all(
					boost::async([&sharedMutex]() {
						std::shared_lock lock(sharedMutex);
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
						}),
					boost::async([&sharedMutex]() {
						std::shared_lock lock(sharedMutex);
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
						})
				).wait();
			});

		EXPECT_TRUE(970 <= elapsedTimeMs && elapsedTimeMs <= 1030);
	}

	TEST(SharedMutexest, ReadWithWriteLockTest) {
		const auto elapsedTimeMs = StopWatch::MeasureMs(
			[]()
			{
				std::shared_mutex sharedMutex;
				boost::when_all(
					boost::async([&sharedMutex]() {
						std::shared_lock lock(sharedMutex);
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
						}),
					boost::async([&sharedMutex]() {
						std::unique_lock lock(sharedMutex);
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
						})
				).wait();
			});

		EXPECT_TRUE(1970 <= elapsedTimeMs && elapsedTimeMs <= 2030);
	}
}

