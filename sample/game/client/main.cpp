#define NOMINMAX

#include "winsock2.h"
#include "../flatbufferschema/player_generated.h"

#include <iostream>
#include <future>
#include <chrono>

#include "../../game/client/GBGameClient.h"

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

	GBGameClient client;

	client.Connect("127.0.0.1", 5076);

	wprintf(L"Connected to server.\n");

	client.AddStub(1, Test);

	while (client.GetKeepLooping())
	{
		std::this_thread::sleep_for(1ms);
	}

	return 0;
}