#include "../../includes/ether.hpp"
#include "../hpp/Client.hpp"

/*void	run_script(Client &client, std::string &body)
{
	std::string			url;
	std::ostringstream	stream;
	std::cout << "run_script\n";

	const char	*cmd = client.getLocConf().cgiparam[0].second.c_str();
	std::string	url = client.getRequest().getUrl();
	const char	*arg = url.substr(url.find_last_of('/') + 1, url.length()).c_str();
	const char	*argv[3] = {cmd, arg, NULL};

	// pid_t	pid;
	// pid = fork();
	// if (pid == 0)
	// {
	// 	dup2(stream.)
	// }
}*/
