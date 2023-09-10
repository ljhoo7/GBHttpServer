#define NOMINMAX

#include "winsock2.h"
#include "../flatbufferschema/player_generated.h"

#include <iostream>
#include <future>
#include <chrono>

using namespace GenericBoson::GameTest;

int main()
{
	// https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-connect
	//----------------------
	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup function failed with error: %d\n", iResult);
		return 1;
	}
	//----------------------
	// Create a SOCKET for connecting to server
	SOCKET ConnectSocket;
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		wprintf(L"socket function failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(5076);

	//----------------------
	// Connect to server.
	iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"connect function failed with error: %ld\n", WSAGetLastError());
		iResult = closesocket(ConnectSocket);
		if (iResult == SOCKET_ERROR)
			wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

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