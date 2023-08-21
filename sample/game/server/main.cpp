#include "../../../game/GBGameServer.h"

#include <future>

int main()
{
	GenericBoson::GBGameServer server(5076);

	std::async([&server]()
		{
			server.AddHandler(1, []() {}, []() {});
		}).get();

		//server.Send();

		return 0;
}