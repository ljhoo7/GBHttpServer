#include "stdafx.h"
#include "GBHttpServer.h"

#include "boost/thread/future.hpp"

#pragma comment(lib, "ws2_32.lib")

using namespace GenericBoson;

int main()
{
	
}

//GBHttpServer srv;

//srv.Start();

//while (true)
//{
//	Sleep(100);
//}
//
//return 0;

//srv.GET("/test/test2", [](RequestQueryMap requestQueryMap, RequestHeaderMap requestHeaderMap, RequestBody requestBody)->GBHttpResponse
//	{
//		std::cout << "here !" << std::endl;

//		//requestBody.ReadFile();

//		GBHttpResponse response(HTTP_STATUS_CODE::OK);

//		//response.WriteFile();

//		return response;
//	});

//PUT("/test/test2", []()
//{
//	std::cout << "here !" << std::endl;

//	//requestBody.ReadFile();

//	//GBHttpResponse response(HTTP_STATUS_CODE::OK);

//	//response.WriteFile();

//	//return response;
//});