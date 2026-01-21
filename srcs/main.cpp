
#include "hpp/Server.hpp"
#include "hpp/Conf.hpp"
#include "../includes/ether.hpp"
#include <signal.h>

bool	server_run = true;

void	convertDnsToIp(IpPortPair &ipport, SrvNameMap &srvmap);

int		test_request(Conf &conf, Server &server)
{
	std::ifstream	file("test_request");
	std::string		input;
	Request			request;

	(void)conf;
	if (file.fail())
		return (std::cout << "cannot open test_request\n" << std::endl, 1);
	std::getline(file, input, '\0');
	std::cout << "\033[0;2m\n-------REQUEST PARSING STARTED------...\n" COLOR_RESET << "\n";
	if (requestParsing(request, input) != 0)
		return (1);
	convertDnsToIp(request, request.getHost(), server.getSrvNameMap());
	std::cout << "\033[0;2mEnd." COLOR_RESET << "\n";
	std::cout << "\033[33m[RESULT]" COLOR_RESET << "\n";
	std::cout << request << "\n";
	if (server.getSrvNameMap().count(request.getHost()) == 0)
		return (std::cout << "server not found\n", 1);
	else
		std:: cout << server.getSrvNameMap()[request.getHost()] << std::endl;
	return (0);
}

void	stopServer(int sig)
{
	std::cout << std::endl << std::endl << "\033[1;31mRequested closing server...\n\033[0m" << std::endl;
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

/*
TODO - 21/01
[X]	RFC request
[ ]	refactoring server
[ ]	refactoring request
*/
int main(int ac, char **av)
{
	std::string	conf_path;

	signal(SIGINT, stopServer);
	std::cout << "\033[1;32mStarting web server ...\033[0m" << std::endl;
	try
	{
		get_conf_path(ac, av, conf_path);
		Conf config(conf_path);
		Server server(config);
		return (test_request(config, server));
		while (server_run)
		{
			int ready = poll(server.getAddrs(), server.getAddrSize(), -1);
			if (ready < 0 && server_run)
				throw std::runtime_error("\033[1;31mPoll ha fallito.\nPorta occupata\n\033[0m");
			for (int i = 0; i < server.getServerNum(); i++)
			{
				if (server.getAddrs()[i].revents & POLLIN)
					server.addSocket(i);
			}
			server.checkForConnection();
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
