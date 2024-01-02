#include <gtest/gtest.h>

#include "../server/GameServer.h"
#include "../core/Shared/ExternalPolymorphism.h"

namespace GenericBoson
{
	struct ExternalPolymorphismTest : public ::testing::Test
	{
		virtual void SetUp() override {}
		static void SetUpTestCase() {}
		static void TearDownTestCase() {}
	};

	TEST_F(ExternalPolymorphismTest, BasicTest)
	{
		Server server(5076);

		Intersection intersection(server, Server::Send);

		EXPECT_TRUE(true);
	}
}

