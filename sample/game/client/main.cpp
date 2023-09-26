#define NOMINMAX

#include "winsock2.h"
#include "../flatbufferschema/player_generated.h"

#include <iostream>
#include <future>
#include <chrono>

#include "../../game/client/GBGameClient.h"

using namespace GenericBoson;
using namespace GenericBoson::GameTest;

int main()
{
	GBGameClient client;

	client.Connect("127.0.0.1", 5076);

	wprintf(L"Connected to server.\n");

	bool keepLooping = false;
	char buffer[1024] = { 0, };
	do {
		const auto receivedBytes = recv(ConnectSocket, buffer, 1024, 0);

		if (receivedBytes)
		{
			std::cout << receivedBytes << " received.\n";

			auto pPlayer = GetPlayer(buffer);

			std::cout << "Name : " << pPlayer->name()->c_str() << '\n';
			std::cout << "Hp : " << pPlayer->hp() << '\n';
		}
	} while (keepLooping);


	iResult = closesocket(ConnectSocket);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	WSACleanup();
	return 0;
}