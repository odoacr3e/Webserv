#include "hpp/Server.hpp"
#include "hpp/Conf.hpp"
#include "../includes/include.hpp"
#include <signal.h>

bool	server_run = true;

void	stopServer(int sig)
{
	LOG_TERM << std::endl << std::endl << "\033[1;31mRequested closing server...\n\033[0m" << std::endl;
	(void)sig, server_run = false;
}

void	get_conf_path(int ac, char **av, std::string &path)
{
	if (ac < 2)
		path = DEFAULT_CONF_PATH;
	else if (ac == 2)
		path = av[1];
	else
		throw std::runtime_error("\033[1;31mToo many configuration files\nPlease pass only one!\033[0m");
}

int main(int ac, char **av, const char **env)
{
	LOG_RESET();
	std::srand(time(0));
	std::string	conf_path;

	signal(SIGINT, stopServer);
	DBG_SRV("\033[32mStarting web server ...");
	try
	{
		get_conf_path(ac, av, conf_path);
		Conf config(conf_path);
		std::cout << config << std::endl;
		Server server(config, env);
		while (server_run)
		{
			int ready = poll(server.getAddrs(), server.getAddrSize(), -1);
			if (ready < 0 && server_run)
				throw std::runtime_error("\033[1;31mPoll ha fallito.\nPorta occupata\n\033[0m");
			for (int i = 0; i < server.getServerNum(); i++)
			{
				if (server.getAddrs()[i].revents & POLLIN)
					server.addSocket(i, FD_CLIENT);
			}
			server.checkForConnection();
		}
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	return (0);
}
