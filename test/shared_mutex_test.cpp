#include <gtest/gtest.h>

namespace GenericBoson
{
	struct SharedMutexest : public ::testing::Test
	{
		virtual void SetUp() override {}
		static void SetUpTestCase() {}
		static void TearDownTestCase() {}
	};

	// Demonstrate some basic assertions.
	TEST(SharedMutexest, BasicAssertions) {
		// Expect two strings not to be equal.
		EXPECT_STRNE("hello", "world");
		// Expect equality.
		EXPECT_EQ(7 * 6, 42);
	}
}

