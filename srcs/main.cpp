
#include "hpp/Server.hpp"
#include "../includes/ether.hpp"
#include <signal.h>

static void	print_fd(struct pollfd *p, int size)
{
	static int	count;

	std::cout << "\033[32m#----------#\033[33m\nIterazione " << count << ":\n\033[0m";  
	for (int i = 0; i < size; i++)
	{
		if (i == 0)
			std::cout << "\033[34mserver:\033[0m\n";
		else
			std::cout << "\033[34mClient" << i << ":\033[0m\n";
		std::cout << "fd:\t" << p[i].fd << "\nevents:\t" << p[i].events << std::endl;
		std::cout << "revents:" << p[i].revents << std::endl << std::endl;
	}
	++count;
}




int	times = 10;

void	ctrl_c(int sig)
{
	std::cout << "Bro stai calmo\n" << std::endl;
	(void)sig, times = 0;
}

int main() //da aggiungere ac e av
{
	int ready;

	signal(SIGINT, ctrl_c);
	std::cout << "\033[32mIl server si spegnerà tra " << times << " secondi.\n\033[0m";
	(void)ready;
	try
	{
		Server server;
		while (times--)
		{
			ready = poll(server.getAddrs(), server.getAddrSize(), -1);
			print_fd(server.getAddrs(), server.getAddrSize());
			if (ready < 0)
				std::cout << "Mannaggia alla madonna poll non va\n";
			if (ready > 0)
				std::cout << "ready: " << ready << std::endl;
			if (server.getAddrs()[0].revents & POLLIN)//FIXME - revents torna sempre 16 (=POLLHUP).
			{											// dovrebbe tornare POLLIN (=1)
				server.addSocket(); // aggiunge al vector il nuovo socket del client
			}
			server.checkForConnection();
			sleep(1);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
