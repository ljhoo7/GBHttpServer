#include "../include/stdafx.h"
#include "../include/GBHttpServer.h"

#pragma comment(lib, "ws2_32.lib")

using namespace GenericBoson;

int main()
{
	GBHttpServer srv;

	//srv.AddGET([]() {}, "/home/test");
	//srv.AddHEAD([]() {}, "/home/test2");
	//srv.AddPOST([]() {}, "/home");


	srv.Start();

	return 0;
}