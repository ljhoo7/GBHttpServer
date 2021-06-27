#include "../include/stdafx.h"
#include "../include/GBHttpServer.h"

#pragma comment(lib, "ws2_32.lib")

using namespace GenericBoson;

int main()
{
	GBHttpServer srv;

	srv.GET("/test/test2", [](int param)->GBHttpResponse
		{
			std::cout << "here !" << std::endl;

			return GBHttpResponse();
		});

	srv.Start();

	while (true)
	{
		Sleep(100);
	}
	
	return 0;
}