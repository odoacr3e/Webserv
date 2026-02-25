#include "../../includes/ether.hpp"
#include "../hpp/Client.hpp"
#include "../hpp/Server.hpp"
#include <sys/wait.h>

typedef std::vector<char *>	argvVector;

static void		get_argv(Client &client, argvVector &v);
static void		run_cmd(Server &srv, Client &client, t_cgi &cgi_data, argvVector &argv);
static void		run_daemon(Server &srv, Client &client, t_cgi &cgi_data, argvVector &argv);
static int 		hex_value(char c);
static void		convert_hexa(std::vector<char*> &input);

std::string		createHtmlPokedex(s_cgi &cgi);
std::string		createHtmlCub(t_cgi &cgi_data, Server &srv, Client &client);
std::string		createHtmlYouTube(t_cgi &cgi_data);
std::string		createHtmlCrypter(t_cgi &cgi_ptr);
void			createArgvCrypter(std::string &args, argvVector &argv);
void			createArgvWeaksleep(std::string &args, argvVector &argv_data);
std::string		createHtmlWeaksleep(t_cgi &cgi_data);

void	run_script(Server &srv, Client &client, std::string &body)
{
	t_cgi				cgi_data(client);
	t_cgi				*cgi_ptr = NULL;
	std::vector<char *> argv;

	std::cout << "RUN_SCRIPT\n";
	if (srv.getFdData()[client.getSockFd()].cgi_ready == false)//prima volta
	{
		if (srv.getFdData()[client.getSockFd()].cgi != NULL)
		{
			std::cerr << "WAIT A SECOND SIR!\n";
			sleep(10);
			return ;
		}
		get_argv(client, argv);
		if (client.getLocConf().script_daemon == true)
			run_daemon(srv, client, cgi_data, argv);
		else
			run_cmd(srv, client, cgi_data, argv);
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
	{
		body = createHtmlCrypter(*cgi_ptr);
	}
	else if (client.getLocConf().script_type == "weaksleep")
		body = createHtmlWeaksleep(*cgi_ptr);
	else
	{
		body = cgi_ptr->output;
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

static void		get_argv(Client &client, argvVector &argv)
{
	std::string	url;
	std::string	cmd;
	std::string	args;
	char		separator;
	char		*temp;

	url = client.getRequest().getUrl();
	url.find('?') != std::string::npos ? separator = '?' : separator = '/';
	if (separator == '?')
		cmd = url.substr(0, url.find_last_of('?'));
	else if (client.getLocConf().cgi_path.length() == 0)
		cmd = "CGI_PATH_NOT_FOUND";
	else
		cmd = client.getLocConf().cgi_path;
	if (client.getRequest().getMethodEnum() == POST)
	{
		client.getRequest().getBinBody().push_back('\0');
		args = client.getRequest().getBinBody().data();
		while (find_and_replace(args, "+", " "));
	}
	else
		args = url.substr(url.find_last_of(separator) + 1, url.length());
	std::cout << "get_argv(): args --->" << args << "\n";
	// www/cgi-bin/crypter/crypter.cgi=0.7
	temp = new char[cmd.length() + 1];
	temp[cmd.length()] = 0;
	std::memcpy(temp, cmd.c_str(), cmd.length());
	argv.push_back(temp);
	if (client.getLocConf().script_type == "crypter")
		createArgvCrypter(args, argv);
	else if (client.getLocConf().script_type == "weaksleep")
		createArgvWeaksleep(args, argv);
	else
	{
		find_and_erase(args, "value=");//FIXME - per pokedex
		vect_split_new(argv, args, separator);
	}
	argv.push_back(NULL);
	std::cout << "cmd: " << argv[0] << std::endl;
	convert_hexa(argv);
	for (size_t i = 1; i < argv.size() - 1; i++)
		std::cout << "arg " << i << ": " << argv[i] << std::endl;
}

	/*std::cout << WHITE"cgi_data.pipe[0] " RED<< cgi_data.pipe[0] << RESET"\n";
	std::cout << WHITE"client_fd " RED << srv.getFdData()[cgi_data.pipe[0]].client->getSockFd() << RESET"\n";
	//std::memcpy(&srv.getFdData()[cgi_data.pipe[0]].cgi_data, &cgi_data, sizeof(t_cgi));
	std::cout << WHITE"client fd orig " RED << cgi_data.client_fd << RESET"\n";
	std::cout << WHITE"client copied: " RED << srv.getFdData()[cgi_data.pipe[0]].cgi_data.client_fd << RESET"\n";*/

static void		run_cmd(Server &srv, Client &client, t_cgi &cgi_data, argvVector &argv)
{
	if (pipe(cgi_data.pipe) != 0)
		return (std::cout << "run_script fatal error: pipe\n", (void)0);
	fcntl(cgi_data.pipe[0], FD_CLOEXEC);
	fcntl(cgi_data.pipe[1], FD_CLOEXEC);
	cgi_data.pid = fork();
	if (cgi_data.pid == -1)
		return (std::cout << "run_script fatal error: pid\n", (void)0);
	else if (cgi_data.pid == 0)
	{
		dup2(cgi_data.pipe[1], STDOUT_FILENO);
		close(cgi_data.pipe[0]);
		close(cgi_data.pipe[1]);
		srv.suppressSocket();
		std::cerr << "VEDIAMO: " << argv[0] << ", " << argv.data()[1] << std::endl;
		std::cerr << "VEDIAMO: " << argv[2] << ", " << (argv.data()[3] == NULL) << std::endl;
		execve(argv[0], argv.data(), NULL);
		for (size_t i = 0; i != argv.size() - 1; i++)
			delete [] argv[i];
		std::cerr << "run_script fatal error: execve failed!\n";
		std::exit(1);
	}
	for (size_t i = 0; i != argv.size() - 1; i++)
		delete [] argv[i];
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
	client.getBuffer().clear();
	client.getRequest().getBinBody().clear();
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

static void		run_daemon(Server &srv, Client &client, t_cgi &cgi_data, argvVector &argv)
{
	ipPortCgiMap::iterator	cgi_exist;

	cgi_exist = srv.getIpPortCgiMap().find(client.getRequest().getHost());
	if (cgi_exist != srv.getIpPortCgiMap().end())
	{
		cgi_data = cgi_exist->second;
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
			execve(argv[0], argv.data(), NULL);
			perror("execve");
			std::cerr << "run_script fatal error: execve\n";
			std::exit(1);
		}
		close(pipes[0][1]);
		close(pipes[1][0]);
		srv.getIpPortCgiMap()[client.getRequest().getHost()] = cgi_data;
		cgi_data.poll_index[0] = srv.addSocket(cgi_data.pipe[0], FD_PIPE_RD);
		srv.getIpPortCgiMap()[client.getRequest().getHost()] = cgi_data;
		cgi_exist = srv.getIpPortCgiMap().find(client.getRequest().getHost());
	}
	/*OLD
	write(cgi_data.pipe[1], cgi_data.argv[1], cgi_data.argv_len[1]);
	std::string	filename("/dev/fd/" + ft_to_string(cgi_data.pipe[0]));
	std::cout << filename << std::endl;
	read_file(filename, client.getBuffer(), 14745718);
	print_file("RESPONSE", client.getBuffer().data(), 1000);
	cgi_data.client = &client;
	*/
	write(cgi_data.pipe[1], argv[1], std::strlen(argv[1]));
	/*
		1)	ADDSOCKET
		2)	client stai zittoListening on -> 10.11.4.3:9020

Can't bind ip:port -> 10.11.4.5:9020
		3)	setting fdData[pipe[0]]
		4)	setting fdData[client]
	*/
	// 1) ADDSOCKET
	// 2 client stai zitto
	srv.getAddrsVector()[cgi_data.poll_index[0]].events = POLLIN;
	client.getPollFd()->events = 0;
	std::cout << WHITE "run_cmd(): client " RESET << client.getSockFd() << " in attesa..\n";
	std::cout << WHITE "run_cmd(): pipe[0] " RESET << cgi_data.pipe[0] << " POLLIN\n";
	// 3) pipe[0]
	s_cgi	*cgi_ptr = &cgi_exist->second;
	srv.getFdData()[cgi_data.pipe[0]].client = &client;
	srv.getFdData()[cgi_data.pipe[0]].cgi = cgi_ptr;
	// 4) client
	srv.getFdData()[client.getSockFd()].cgi_ready = true;
	srv.getFdData()[client.getSockFd()].client = &client;
	srv.getFdData()[client.getSockFd()].cgi = cgi_ptr;
}

static int hex_value(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return -1;
}

void convert_hexa(std::vector<char*> &input)
{
    for (size_t idx = 0; idx < input.size(); ++idx)
    {
        char *original = input[idx];
		if(!original)
			continue ;

        size_t len = std::strlen(original);
        char *decoded = new char[len + 1];
        size_t read = 0;
        size_t write = 0;

        while (read < len)
        {
            if (original[read] == '%' && read + 2 < len)
            {
                int high = hex_value(original[read + 1]);
                int low  = hex_value(original[read + 2]);
                if (high != -1 && low != -1)
                {
                    decoded[write++] = static_cast<char>((high << 4) | low);
                    read += 3;
                    continue;
                }
            }
            decoded[write++] = original[read++];
        }
        decoded[write] = '\0';

        delete[] original;
        input[idx] = decoded;
    }
}
