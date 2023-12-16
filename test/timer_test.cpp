#include <gtest/gtest.h>

#include "../core/Shared/Timer.h"

#include <memory>
#include <atomic>
#include <chrono>

namespace GenericBoson
{
	

	class TestTimer : public ITimer, public std::enable_shared_from_this<TestTimer>
	{
	public:
		TestTimer(const int64_t periodMs) : ITimer(periodMs), m_StartTime(std::chrono::system_clock::now()) {}

		void OnTime() override
		{
			TimerManager::GetInstance()->AddTimer(shared_from_this());
			m_Count += 1;
		}
	public:
		std::atomic_int m_Count = 0;
		std::chrono::system_clock::time_point m_StartTime;
	};

	struct TimerTest : public ::testing::Test
	{
		virtual void SetUp() override {}
		static void SetUpTestCase() {}
		static void TearDownTestCase() {}
	};

	TEST_F(TimerTest, BasicTest) {
		const auto pTimer = std::make_shared<TestTimer>(100);
		TimerManager::GetInstance()->AddTimer(pTimer);
		std::this_thread::sleep_for(std::chrono::milliseconds(1100));
		const auto elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - pTimer->m_StartTime).count();
		int countCopy = pTimer->m_Count.load();
		EXPECT_GT(countCopy, 9);
		EXPECT_LT(countCopy, 11);
	}
}

