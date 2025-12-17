
#include "hpp/Server.hpp"
#include "hpp/Conf.hpp"
#include "../includes/ether.hpp"
#include <signal.h>

// static void	print_fd(struct pollfd *p, int size)
// {
// 	static int	count;

// 	std::cout << "\033[32m#----------#\033[33m\nIterazione " << count << ":\n\033[0m";  
// 	for (int i = 0; i < size; i++)
// 	{
// 		if (i == 0)
// 			std::cout << "\033[34mserver:\033[0m\n";
// 		else
// 			std::cout << "\033[34mClient" << i << ":\033[0m\n";
// 		std::cout << "fd:\t" << p[i].fd << "\nevents:\t" << p[i].events << std::endl;
// 		std::cout << "revents:" << p[i].revents << std::endl << std::endl;
// 	}
// 	++count;
// }std::strchr("/{};", line[i])

int	times = 1000;

void	spread_democracy(int sig)
{
	std::cout << "\nONOLULU ARRIVO!\n" << std::endl;
	(void)sig, times = 0;
}

int main(int ac, char **av) //da aggiungere ac e av
{
	signal(SIGINT, spread_democracy);
	std::cout << "\033[32mIl server si spegnerà tra " << times << " secondi.\n\033[0m";
	try
	{
		if (ac < 2)
			Conf conf(CONF_DEFAULT_PATH);
		else if (ac == 2)
			Conf conf(av[1]);
		else
			throw std::runtime_error("\033[31mToo many configuration files\nPlease pass only one!\033[0m");
		Server server;
		while (times--)
		{
			int ready = poll(server.getAddrs(), server.getAddrSize(), -1);
			if (ready < 0 && times != 0)
				throw std::runtime_error("\033[31mPoll ha fallito.\nPorta occupata\n\033[0m");
			if (server.getAddrs()[0].revents & POLLIN)
				server.addSocket(); // aggiunge al vector il nuovo socket del client
			server.checkForConnection();
			// sleep(1);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
