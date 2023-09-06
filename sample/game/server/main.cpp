#define NOMINMAX

#include "../../../game/GBGameServer.h"
#include "../flatbufferschema/player_generated.h"

#include <iostream>
#include <future>
#include <chrono>

using namespace GenericBoson::GameTest;

int main()
{
	using namespace std::literals::chrono_literals;

	GenericBoson::GBGameServer server(5076);

	server.AddHandler(1, []()
		-> std::optional<::flatbuffers::FlatBufferBuilder> {
			::flatbuffers::FlatBufferBuilder fbb;
			auto name = fbb.CreateString("Slime");
			auto pPlayer = CreatePlayer(fbb, nullptr, 99, name);

			fbb.Finish(pPlayer);

			return fbb;
		},
		[](const char* rawBuffer) -> bool {
			const auto* pPlayer = GetPlayer(rawBuffer);

			std::cout << "hp : " << pPlayer->hp() << '\n';
			std::cout << "name : " << pPlayer->name() << '\n';

			return true;
		});

	if (const auto errorMsg = server.Start(); errorMsg.empty())
	{
		while (server.GetKeepLooping())
		{
			std::this_thread::sleep_for(1min);
		}
	}

	return 0;
}