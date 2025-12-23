
#include "hpp/Server.hpp"
#include "hpp/Conf.hpp"
#include "../includes/ether.hpp"
#include <signal.h>

bool	times = true;

void	spread_democracy(int sig)
{
	std::cout << "\nONOLULU ARRIVO!\n" << std::endl;
	(void)sig, times = false;
}

int main(int ac, char **av) //da aggiungere ac e av
{
	std::string	conf_path;

	signal(SIGINT, spread_democracy);
	std::cout << "\033[32mIl server si spegnerà tra " << times << " secondi.\n\033[0m";
	try
	{
		if (ac < 2)
			conf_path = DEFAULT_CONF_PATH;
		else if (ac == 2)
			conf_path = av[1];
		else
			throw std::runtime_error("\033[31mToo many configuration files\nPlease pass only one!\033[0m");
		Conf config(conf_path);
		Server server(config);
		while (times)
		{
			int ready = poll(server.getAddrs(), server.getAddrSize(), -1);
			if (ready < 0 && times != 0)
				throw std::runtime_error("\033[31mPoll ha fallito.\nPorta occupata\n\033[0m");
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
