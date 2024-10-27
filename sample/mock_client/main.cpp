#define NOMINMAX

#include "../flatbufferschema/player_generated.h"

#include <iostream>
#include <future>
#include <chrono>

#include "../../engine/client/Client.h"

using namespace GenericBoson;
using namespace GenericBoson::GameTest;

void Test(const ::GenericBoson::GameTest::Player& pPlayer)
{
	std::cout << "hp : " << pPlayer.hp() << '\n';
	std::cout << "name : " << pPlayer.name()->c_str() << '\n';
}

int main()
{
	using namespace std::literals::chrono_literals;

	Client client;

	if (int result = client.Connect("127.0.0.1", 5076))
	{
		std::cout << std::format("Connect to server failed. error - {}\n", result);
		return -1;
	}

	wprintf(L"Connected to server.\n");

	client.AddStub(1, Test);

	std::this_thread::sleep_for(12ms);
	while (client.GetKeepLooping())
	{
		std::this_thread::sleep_for(1ms);
	}

	return 0;
}