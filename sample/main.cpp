#include "../include/stdafx.h"
#include "../include/GBHttpServer.h"

#pragma comment(lib, "ws2_32.lib")

using namespace GenericBoson;

int main()
{
	GBHttpServer srv;

	srv.GET("/test/test2", [](GB_ACTION_METHOD_PARAMETERS)->GBHttpResponse
		{
			std::cout << "here !" << std::endl;

			return GBHttpResponse(HTTP_STATUS_CODE::OK);
		});

	srv.Start();

	while (true)
	{
		Sleep(100);
	}
	
	return 0;
}