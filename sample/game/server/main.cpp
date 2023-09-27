#define NOMINMAX

#include "../../core/server/GBExpandedOverlapped.h"
#include "../../game/server/GBGameServer.h"

#include "../flatbufferschema/player_generated.h"

#include <iostream>
#include <future>
#include <chrono>

using namespace GenericBoson;
using namespace GenericBoson::GameTest;

void Test(const Player& player)
{
	std::cout << "hp : " << player.hp() << '\n';
	std::cout << "name : " << player.name() << '\n';
}

int main()
{
	using namespace std::literals::chrono_literals;

	GBGameServer server(5076);

	server.AddStub(1, Test);

	server.SetConnectedTask([&server](auto pEol) {
		server.Send(pEol, 1, 
			[](auto& fbb){
				auto name = fbb.CreateString("Slime");
				auto pPlayer = CreatePlayer(fbb, nullptr, 99, name);

				return pPlayer;
			});
		});

	if (const auto errorMsg = server.Start(); errorMsg.empty())
	{
		std::this_thread::sleep_for(100ms);

		while (server.GetKeepLooping())
		{
			std::this_thread::sleep_for(1ms);
		}
	}
	else
	{
		std::cout << "Error occured : " << errorMsg << std::endl;
	}

	return 0;
}