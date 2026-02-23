#include "../../includes/ether.hpp"
#include "../hpp/Client.hpp"
#include "../hpp/Server.hpp"
#include <sys/wait.h>

typedef std::vector<std::pair<char *, size_t> >	argvVector;

static void		get_argv(Client &client, argvVector &v);
static void		run_cmd(Server &srv, Client &client, t_cgi &cgi_data, argvVector &argv_data);

// static void		run_daemon(Server &srv, Client &client, t_cgi &cgi_data);
std::string		createHtmlPokedex(s_cgi &cgi);
std::string		createHtmlCub(t_cgi &cgi_data, Server &srv, Client &client);
std::string		createHtmlYouTube(t_cgi &cgi_data);
std::string		createHtmlCrypter(t_cgi &cgi_ptr);

void	run_script(Server &srv, Client &client, std::string &body)
{
	t_cgi									cgi_data(client);
	t_cgi									*cgi_ptr = NULL;
	std::vector<std::pair<char *, size_t> > argv_data;

	std::cout << "RUN_SCRIPT\n";
	if (srv.getFdData()[client.getSockFd()].cgi_ready == false)//prima volta
	{
		get_argv(client, argv_data);
		//if (client.getLocConf().script_daemon == true)
		//	;/* run_daemon(srv, client, cgi_data);*/
		//else
		run_cmd(srv, client, cgi_data, argv_data);
		return ;
	}
	else // se cgi gia eseguita, ripesca dati cgi
		cgi_ptr = srv.getFdData()[client.getSockFd()].cgi;
	client.getRequest().setBodyType("text/html");
	if (client.getLocConf().script_type == "pokedex")
		body = createHtmlPokedex(*cgi_ptr);
	else if (client.getLocConf().script_type == "cub3D")
		body = createHtmlCub(*cgi_ptr, srv, client);
	else if (client.getLocConf().script_type == "giorgio")
		body = createHtmlYouTube(*cgi_ptr);
	else if (client.getLocConf().script_type == "crypter")
		body = createHtmlCrypter(*cgi_ptr);
	else
	{
		client.getRequest().setBodyType("text/plain");
		std::cout << "script_type undefined. no html created." << std::endl;
		std::cout << cgi_data.output << std::endl;
	}
	// std::cout << "BODY: \n\n" << BLUE"" << body << RESET << std::endl; 
	if (client.getLocConf().script_daemon == false)
		delete cgi_ptr;
	srv.getFdData()[client.getSockFd()].cgi = NULL;
	srv.getFdData()[client.getSockFd()].cgi_ready = false;
}

static void		get_argv(Client &client, argvVector &argv_data)
{
	std::string					url;
	std::string					cmd;
	std::string					args;
	char						separator;
	std::pair<char *, size_t>	temp_pair;

	url = client.getRequest().getUrl();
	if (url.find('?') != std::string::npos)
	{
		cmd = url.substr(0, url.find_last_of('?'));
		args = url.substr(url.find_last_of('?') + 1, url.length());
		separator = '&';
	}
	else if (client.getRequest().getMethodEnum() == POST)
	{
		// text=1&abc+def&gabibbo+fortissimo&gabibbone
		std::string body = client.getRequest().getBinBody().data();
		std::string	content = body;;
		find_and_replace(content, "&action=", "");
		find_and_replace(body, "text=", "");
		std::string real_content = content.substr(content.find("text=") + 5);
		std::cout << "Real content: " << real_content << std::endl;
		std::vector<std::string> format_real_for_real;
		vect_split(format_real_for_real, real_content, '+');
	}
	else
	{
		if (client.getLocConf().cgiparam.size() == 0)
			cmd = "CGI_PATH_NOT_FOUND";
		else
			cmd = client.getLocConf().cgiparam[0].second;
		args = url.substr(url.find_last_of('/') + 1, url.length());
		separator = '+';
	}
	temp_pair.first = new char[cmd.length()];
	std::memcpy(temp_pair.first, cmd.c_str(), cmd.length());
	argv_data.push_back(temp_pair);
	find_and_replace(args, "value=", "");
	vect_split(argv_data, args, separator);
	temp_pair.first = NULL;
	temp_pair.second = 0;
	argv_data.push_back(temp_pair);
	std::cout << "cmd: " << argv_data[0].first << "\n";
	for (size_t i = 1; i < argv_data.size() - 1; i++)
	{
		std::cout << "arg " << i << ": " << argv_data[i].first << "\n";
	}
}

	/*std::cout << WHITE"cgi_data.pipe[0] " RED<< cgi_data.pipe[0] << RESET"\n";
	std::cout << WHITE"client_fd " RED << srv.getFdData()[cgi_data.pipe[0]].client->getSockFd() << RESET"\n";
	//std::memcpy(&srv.getFdData()[cgi_data.pipe[0]].cgi_data, &cgi_data, sizeof(t_cgi));
	std::cout << WHITE"client fd orig " RED << cgi_data.client_fd << RESET"\n";
	std::cout << WHITE"client copied: " RED << srv.getFdData()[cgi_data.pipe[0]].cgi_data.client_fd << RESET"\n";*/

std::vector<char *>	extract_args(argvVector &argv)
{
	std::vector<char *>	ret;

	for (size_t i = 0; i < argv.size(); i++)
	{
		ret.push_back(argv[i].first);
	}
	return (ret);
}

static void		run_cmd(Server &srv, Client &client, t_cgi &cgi_data, argvVector &argv_data)
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

		execve(argv_data[0].first, extract_args(argv_data).data(), NULL);
		std::cerr << "run_script fatal error\n";
		std::exit(1);
	}
	std::cout << WHITE"Entro e chiudo pipe[0]: " RED << cgi_data.pipe[1] << "" RESET << std::endl;
	close_fd(&cgi_data.pipe[1]);
	std::string	filename("/dev/fd/" + ft_to_string(cgi_data.pipe[0]));
	std::cout << filename << std::endl;
	/*
		1)	ADDSOCKET
		2)	client stai zitto
		3)	setting fdData[pipe[0]]
		4)	setting fdData[client]
	*/
	// 1) ADDSOCKET
	cgi_data.poll_index[0] = srv.addSocket(cgi_data.pipe[0], FD_PIPE_RD);
	// 2 client stai zitto
	client.getPollFd()->events = 0;
	std::cout << WHITE "run_cmd(): client " RESET << client.getSockFd() << " in attesa..\n";
	std::cout << WHITE "run_cmd(): pipe[0] " RESET << cgi_data.pipe[0] << " POLLIN\n";
	// 3) pipe[0]
	s_cgi	*cgi_ptr = new s_cgi(cgi_data);
	srv.getFdData()[cgi_data.pipe[0]].client = &client;
	srv.getFdData()[cgi_data.pipe[0]].cgi = cgi_ptr;
	// 4) client
	srv.getFdData()[client.getSockFd()].cgi_ready = true;
	srv.getFdData()[client.getSockFd()].client = &client;
	srv.getFdData()[client.getSockFd()].cgi = cgi_ptr;
}

int read_file(std::string name, std::vector<char> &vect, int bytes);

// static void		run_daemon(Server &srv, Client &client, t_cgi &cgi_data)
// {
// 	ipPortCgiMap::iterator	cgi_exist;
// 	std::string 			argv[2];

// 	cgi_exist = srv.getIpPortCgiMap().find(client.getRequest().getHost());
// 	if (cgi_exist != srv.getIpPortCgiMap().end())
// 	{
// 		cgi_data = cgi_exist->second;
// 		get_argv(client, cgi_data, argv);
// 	}
// 	else
// 	{
// 		int		pipes[2][2];

// 		if (pipe(pipes[0]) != 0 || pipe(pipes[1]) != 0)
// 			return (std::cout << "run_script fatal error: pipe\n", (void)0);
// 		cgi_data.pipe[0] = pipes[0][0];
// 		cgi_data.pipe[1] = pipes[1][1];
// 		cgi_data.pid = fork();
// 		if (cgi_data.pid == -1)
// 			return (std::cout << "run_script fatal error: fork\n", (void)0);
// 		else if (cgi_data.pid == 0)
// 		{
// 			dup2(pipes[0][1], STDOUT_FILENO);
// 			dup2(pipes[1][0], STDIN_FILENO);
// 			close(pipes[0][0]);
// 			close(pipes[1][1]);
// 			srv.suppressSocket();
// 			execve(cgi_data.argv[0], cgi_data.argv, NULL);
// 			perror("execve");
// 			std::cerr << "run_script fatal error: execve\n";
// 			std::exit(1);
// 		}
// 		close(pipes[0][1]);
// 		close(pipes[1][0]);
// 		srv.getIpPortCgiMap()[client.getRequest().getHost()] = cgi_data;
// 		cgi_data.poll_index[0] = srv.addSocket(cgi_data.pipe[0], FD_PIPE_RD);
// 		cgi_exist = srv.getIpPortCgiMap().find(client.getRequest().getHost());
// 	}
// 	/*OLD
// 	write(cgi_data.pipe[1], cgi_data.argv[1], cgi_data.argv_len[1]);
// 	std::string	filename("/dev/fd/" + ft_to_string(cgi_data.pipe[0]));
// 	std::cout << filename << std::endl;
// 	read_file(filename, client.getBuffer(), 14745718);
// 	print_file("RESPONSE", client.getBuffer().data(), 1000);
// 	cgi_data.client = &client;
// 	*/
// 	write(cgi_data.pipe[1], cgi_data.argv[1], cgi_data.argv_len[1]);
// 	/*
// 		1)	ADDSOCKET
// 		2)	client stai zitto
// 		3)	setting fdData[pipe[0]]
// 		4)	setting fdData[client]
// 	*/
// 	// 1) ADDSOCKET
// 	// 2 client stai zitto
// 	srv.getAddrsVector()[cgi_data.pipe[0]].events = POLLIN;
// 	client.getPollFd()->events = 0;
// 	std::cout << WHITE "run_cmd(): client " RESET << client.getSockFd() << " in attesa..\n";
// 	std::cout << WHITE "run_cmd(): pipe[0] " RESET << cgi_data.pipe[0] << " POLLIN\n";
// 	// 3) pipe[0]
// 	s_cgi	*cgi_ptr = &cgi_exist->second;
// 	srv.getFdData()[cgi_data.pipe[0]].client = &client;
// 	srv.getFdData()[cgi_data.pipe[0]].cgi = cgi_ptr;
// 	// 4) client
// 	srv.getFdData()[client.getSockFd()].cgi_ready = true;
// 	srv.getFdData()[client.getSockFd()].client = &client;
// 	srv.getFdData()[client.getSockFd()].cgi = cgi_ptr;
// }
