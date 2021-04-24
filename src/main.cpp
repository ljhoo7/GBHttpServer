#include "../include/stdafx.h"
#include "../include/GBHttpServer.h"
#include "main.h"

#pragma comment(lib, "ws2_32.lib")

using namespace GenericBoson;

int main()
{
	GBHttpServer srv;

	////srv.Add<GET>([](int arg) {});
	////srv.AddHEAD([]() {}, "/home/test2");
	////srv.AddPOST([]() {}, "/home");

	srv.GET("/test/test2", [](int param)
		{
			std::cout << "here !" << std::endl;
		});

	srv.Start();
	
	return 0;
}