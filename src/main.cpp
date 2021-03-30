#include "../include/stdafx.h"
#include "../include/GBHttpServer.h"

#pragma comment(lib, "ws2_32.lib")

using namespace GenericBoson;

int main()
{
	GBHttpServer srv;
	srv.Start();

	return 0;
}