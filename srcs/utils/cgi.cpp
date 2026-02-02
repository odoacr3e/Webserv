
#include "../../includes/ether.hpp"
#include "../hpp/Client.hpp"
#include "../hpp/Server.hpp"
#include <sys/wait.h>

static void		get_argv(Client &client, std::string argv[2], std::string &url);
static void		run_cmd(Server &srv, char *const argv[], std::string &output);
std::string		createHtmlPokedex(std::string &key, std::string &output);

void	run_script(Server &srv, Client &client, std::string &body)
{
	std::string	argv_str[2];
	std::string	url;

	std::cout << "run_script\n";
	get_argv(client, argv_str, url);
	char const	*argv[3] = {argv_str[0].c_str(), argv_str[1].c_str(), NULL};
	std::cout << "cmd: " << argv[0] << "\n";
	std::cout << "arg: " << argv[1] << "\n";
	run_cmd(srv, (char *const *)argv, body);
	client.getRequest().setBodyType("text/html");
	if (client.getLocConf().script_type == "pokedex")
		body = createHtmlPokedex(argv_str[1], body);
	else
	{
		client.getRequest().setBodyType("text/plain");
		std::cout << "script_type undefined. no html created." << std::endl;
	}
}

static void		get_argv(Client &client, std::string argv[2], std::string &url)
{
	url = client.getRequest().getUrl();
	if (url.find('?'))
	{
		argv[0] = url.substr(0, url.find_last_of('?'));
		argv[1] = url.substr(url.find_last_of('?') + 1, url.length());
		find_and_replace(argv[1], "value=", "");
		url = url_arg_remove(client.getRequest().getUrlOriginal(), '=');
	}
	else
	{
		argv[0] = client.getLocConf().cgiparam[0].second;
		argv[1] = url.substr(url.find_last_of('/') + 1, url.length());
		url = argv[0] + '/';
		url = url_arg_remove(client.getRequest().getUrlOriginal(), '/');
	}
}

static void	run_cmd(Server &srv, char *const argv[], std::string &output)
{
	std::string			url;
	int					pipe_fd[2];

	pid_t	pid;
	if (pipe(pipe_fd) != 0)
		return (std::cout << "run_script fatal error\n", (void)0);
	pid = fork();
	if (pid == -1)
		return (std::cout << "run_script fatal error\n", (void)0);
	else if (pid == 0)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		srv.suppressSocket();
		execve(argv[0], argv, NULL);
		std::cerr << "run_script fatal error\n";
		std::exit(1);
	}
	close(pipe_fd[1]);
	wait(NULL);
	std::string	filename("/dev/fd/" + ft_to_string(pipe_fd[0]));
	std::cout << filename << std::endl;
	std::ifstream	output_fd(filename.c_str(), std::ios_base::in);
	std::getline(output_fd, output, '\0');
	close(pipe_fd[0]);
}
