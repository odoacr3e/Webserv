#include "hpp/Server.hpp"
#include "hpp/Conf.hpp"
#include "../includes/ether.hpp"
#include <signal.h>

bool	server_run = true;

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

int main(int ac, char **av, const char **env)
{
	std::remove("REQUEST");//FIXME - togliere, solo per debug
	std::string	conf_path;

	signal(SIGINT, stopServer);
	std::cout << "\033[1;32mStarting web server ...\033[0m" << std::endl;
	try
	{
		get_conf_path(ac, av, conf_path);
		Conf config(conf_path);
		std::cout << config << std::endl;
		Server server(config, env);
		// return (test_request(config, server));
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
		std::cout << e.what() << '\n';
	}
	return (0);
}

/*
	[]	upload con autorizzazioni, html bellino
	[]	cgi non bloccanti
	[]	cookie
	[]	refactoring
	[]	test
*/
/*
- 	Your server must remain non-blocking at all times 
	and properly handle client disconnections when 
	necessary.
X	Clients must be able to upload files.
	Page with delete and post.
X	You need at least the GET, POST, and DELETE methods
-	Stress test your server to ensure 
	it remains available at all times.
-	Uploading files from the clients to the server is 
	authorized, and storage location is provided
-	Just remember that, for chunked requests,
	your server needs to un-chunk them, 
	the CGI will expect EOF as the end of the body.
- 	The same applies to the output of the CGI. 
	If no content_length is returned from the CGI, 
	EOF will mark the end of the returned data.
X	You must provide configuration files and default 
	files to test and demonstrate that every feature 
	works during the evaluation.
-	We have provided a small tester. 
	Using it is not mandatory if everything works fine 
	with your browser and tests, but it can help
	you find and fix bugs.
BONUS:
-	Support cookies and session management 
	(provide simple examples).
-	Handle multiple CGI types.
OTHER:
-	game cgi
-	crypter
*/

// int	main(int ac, char **av)
// {
// 	std::string str;

// 	if (ac == 1)
// 		ft_ls("www/", str);
// 	else
// 		ft_ls(av[1], str);
// 	std::cout << str << std::endl;
// }
