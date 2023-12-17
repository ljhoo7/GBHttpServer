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

		template<typename MUTEX>
		void LockSleep(MUTEX& mutex)
		{
			std::shared_lock lock();
			
		}
	};

	TEST(SharedMutexest, ReadLockOnlyTest) {
		std::shared_mutex sharedMutex;

		const auto readFuture1 = std::async([&sharedMutex]() {
				std::shared_lock lock(sharedMutex);
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			});
		
		const auto readFuture2 = std::async([&sharedMutex]() {
				std::shared_lock lock(sharedMutex);
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			});

		
	}

	TEST(SharedMutexest, ReadWithWriteLockTest) {
		std::shared_mutex sharedMutex;
		const auto writeFuture = std::async([&sharedMutex]() {
				std::lock_guard lock(sharedMutex);
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			});

		const auto readFuture = std::async([&sharedMutex]() {
				std::shared_lock lock(sharedMutex);
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			});

		EXPECT_STRNE("hello", "world");
		EXPECT_EQ(7 * 6, 42);
	}
}

