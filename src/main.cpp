#include "../include/stdafx.h"
#include "../include/GBHttpServer.h"

#pragma comment(lib, "ws2_32.lib")

using namespace GenericBoson;

int main()
{
	GBHttpServer srv;

	srv.GET("/test/test2", [](int param)
		{
			std::cout << "here !" << std::endl;
		});

	srv.GET("/test/test2", [](int param)
		{
			std::cout << "here !" << std::endl;
		});

	srv.GET("/test/test2", [](int param)
		{
			std::cout << "here !" << std::endl;
		});

	srv.GET("/test/test2", [](int param)
		{
			std::cout << "here !" << std::endl;
		});

	srv.Start();
	
	return 0;
}