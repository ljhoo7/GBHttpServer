#include <gtest/gtest.h>

#include "../game/server/GameServer.h"
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
		GameServer server(5076);

		Intersection intersection(server, &GameServer::SendPing);

		ExpandedOverlapped eol;
		SendPing(intersection, &eol);

		EXPECT_TRUE(true);
	}
}

