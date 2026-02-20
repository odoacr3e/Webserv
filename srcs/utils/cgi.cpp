#include "../../includes/ether.hpp"
#include "../hpp/Client.hpp"
#include "../hpp/Server.hpp"
#include <sys/wait.h>

static void		get_argv(Client &client, t_cgi &cgi_data, std::string argv[2]);
static void		run_cmd(Server &srv, Client &client, t_cgi &cgi_data);
static void		run_daemon(Server &srv, Client &client, t_cgi &cgi_data);
std::string		createHtmlPokedex(s_cgi &cgi);
std::string		createHtmlCub(t_cgi &cgi_data, Server &srv, Client &client);
std::string		createHtmlYouTube(t_cgi &cgi_data);

void	run_script(Server &srv, Client &client, std::string &body)
{
	t_cgi					cgi_data(client);
	t_cgi					*cgi_ptr = NULL;
	std::string 			argv[2];

	std::cout << "run_script\n";
	if (srv.getFdData()[client.getSockFd()].cgi_ready == false)
	{
		get_argv(client, cgi_data, argv);
		if (client.getLocConf().script_daemon == true)
			run_daemon(srv, client, cgi_data);
		else
			run_cmd(srv, client, cgi_data);
		return ;
	}
	else
		cgi_ptr = srv.getFdData()[client.getSockFd()].cgi;
	client.getRequest().setBodyType("text/html");
	if (client.getLocConf().script_type == "pokedex")
		body = createHtmlPokedex(*cgi_ptr);
	else if (client.getLocConf().script_type == "cub3D")
		body = createHtmlCub(*cgi_ptr, srv, client);
	else if (client.getLocConf().script_type == "giorgio")
		body = createHtmlYouTube(*cgi_ptr);
	else
	{
		client.getRequest().setBodyType("text/plain");
		std::cout << "script_type undefined. no html created." << std::endl;
		std::cout << cgi_data.output << std::endl;
	}
	delete cgi_ptr;
	srv.getFdData()[client.getSockFd()].cgi = NULL;
	srv.getFdData()[client.getSockFd()].cgi_ready = false;
}

static void		get_argv(Client &client, t_cgi &cgi_data, std::string argv[2])
{
	std::string	url;

	url = client.getRequest().getUrl();
	if (url.find('?') != std::string::npos)
	{
		argv[0] = url.substr(0, url.find_last_of('?'));
		argv[1] = url.substr(url.find_last_of('?') + 1, url.length());
		find_and_replace(argv[1], "value=", "");
	}
	else
	{
		argv[0] = client.getLocConf().cgiparam[0].second;
		argv[1] = url.substr(url.find_last_of('/') + 1, url.length());
	}
	std::cout << "cmd: " << argv[0] << "\n";
	std::cout << "arg: " << argv[1] << "\n";
	cgi_data.argv[0] = (char *)(void *)argv[0].c_str();
	cgi_data.argv_len[0] = argv[0].length();
	cgi_data.argv[1] = (char *)(void *)argv[1].c_str();
	cgi_data.argv_len[1] = argv[1].length();
	cgi_data.argv[2] = NULL;
}

	/*std::cout << WHITE"cgi_data.pipe[0] " RED<< cgi_data.pipe[0] << RESET"\n";
	std::cout << WHITE"client_fd " RED << srv.getFdData()[cgi_data.pipe[0]].client->getSockFd() << RESET"\n";
	//std::memcpy(&srv.getFdData()[cgi_data.pipe[0]].cgi_data, &cgi_data, sizeof(t_cgi));
	std::cout << WHITE"client fd orig " RED << cgi_data.client_fd << RESET"\n";
	std::cout << WHITE"client copied: " RED << srv.getFdData()[cgi_data.pipe[0]].cgi_data.client_fd << RESET"\n";*/

static void		run_cmd(Server &srv, Client &client, t_cgi &cgi_data)
{
	if (pipe(cgi_data.pipe) != 0)
		return (std::cout << "run_script fatal error\n", (void)0);
	cgi_data.pid = fork();
	if (cgi_data.pid == -1)
		return (std::cout << "run_script fatal error\n", (void)0);
	else if (cgi_data.pid == 0)
	{
		dup2(cgi_data.pipe[1], STDOUT_FILENO);
		close(cgi_data.pipe[0]);
		close(cgi_data.pipe[1]);
		srv.suppressSocket();
		execve(cgi_data.argv[0], cgi_data.argv, NULL);
		std::cerr << "run_script fatal error\n";
		std::exit(1);
	}
	close_fd(&cgi_data.pipe[1]);
	std::string	filename("/dev/fd/" + ft_to_string(cgi_data.pipe[0]));
	std::cout << filename << std::endl;
	/*
		1)	ADDSOCKET
		2)	client stai zitto
		3)	setting fdData[pipe[0]]
		4)	setting fdData[client]
	*/
	// 1
	cgi_data.poll_index[0] = srv.addSocket(cgi_data.pipe[0], FD_PIPE_RD);
	// 2 client stai zitto
	client.getPollFd()->events = 0;

	std::cout << WHITE "run_cmd(): client " RESET << client.getSockFd() << " in attesa..\n";
	std::cout << WHITE "run_cmd(): pipe[0] " RESET << cgi_data.pipe[0] << " POLLIN\n";
	// 3
	s_cgi	*cgi_ptr = new s_cgi(cgi_data);
	srv.getFdData()[cgi_data.pipe[0]].client = &client;
	srv.getFdData()[cgi_data.pipe[0]].cgi = cgi_ptr;
	// 4
	srv.getFdData()[client.getSockFd()].cgi_ready = true;
	srv.getFdData()[client.getSockFd()].client = &client;
	srv.getFdData()[client.getSockFd()].cgi = cgi_ptr;
}

int read_file(std::string name, std::vector<char> &vect, int bytes);

static void		run_daemon(Server &srv, Client &client, t_cgi &cgi_data)
{
	ipPortCgiMap::iterator	cgi_exist;
	std::string 			argv[2];

	cgi_exist = srv.getIpPortCgiMap().find(client.getRequest().getHost());
	if (cgi_exist != srv.getIpPortCgiMap().end())
	{
		cgi_data = cgi_exist->second;
		get_argv(client, cgi_data, argv);
	}
	else
	{
		int		pipes[2][2];

		if (pipe(pipes[0]) != 0 || pipe(pipes[1]) != 0)
			return (std::cout << "run_script fatal error: pipe\n", (void)0);
		cgi_data.pipe[0] = pipes[0][0];
		cgi_data.pipe[1] = pipes[1][1];
		cgi_data.pid = fork();
		if (cgi_data.pid == -1)
			return (std::cout << "run_script fatal error: fork\n", (void)0);
		else if (cgi_data.pid == 0)
		{
			dup2(pipes[0][1], STDOUT_FILENO);
			dup2(pipes[1][0], STDIN_FILENO);
			close(pipes[0][0]);
			close(pipes[1][1]);
			srv.suppressSocket();
			execve(cgi_data.argv[0], cgi_data.argv, NULL);
			perror("execve");
			std::cerr << "run_script fatal error: execve\n";
			std::exit(1);
		}
		close(pipes[0][1]);
		close(pipes[1][0]);
		srv.getIpPortCgiMap()[client.getRequest().getHost()] = cgi_data;
	}
	write(cgi_data.pipe[1], cgi_data.argv[1], cgi_data.argv_len[1]);
	std::string	filename("/dev/fd/" + ft_to_string(cgi_data.pipe[0]));
	std::cout << filename << std::endl;
	read_file(filename, client.getBuffer(), 14745718);
	print_file("RESPONSE", client.getBuffer().data(), 1000);
	cgi_data.client = &client;
}
