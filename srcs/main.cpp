
#include "hpp/Server.hpp"
#include "../includes/ether.hpp"

int main() //da aggiungere ac e av
{
	try
	{
		Server server;
		while (1)
		{
			int ready = poll(server.getAddrs(), server.getAddrSize(), -1);
			if (server.getAddrs()[0].revents & POLLIN)
				server.addSocket(); // aggiunge al vector il nuovo socket del client
			server.checkForConnection();
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}