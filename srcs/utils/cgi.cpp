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

void	vect_split_free(std::vector<char *> &vect, size_t size);

void	run_script(Server &srv, Client &client, std::string &body)
{
	t_cgi				cgi_data(client);
	t_cgi				*cgi_ptr = NULL;
	std::vector<char *> argv;

	std::cout << "RUN_SCRIPT\n";
	if (srv.getFdData()[client.getSockFd()].cgi_ready == false)//prima volta
	{
		get_argv(client, argv);
		if (client.getLocConf().fastcgi_bool == true)
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
	if (client.getLocConf().fastcgi_bool == false)
		delete cgi_ptr;
	else
		cgi_ptr->reset();
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
		execve(argv[0], argv.data(), NULL);
		vect_split_free(argv, argv.size() - 1);
		std::cerr << "run_script fatal error: execve failed!\n";
		std::exit(1);
	}
	vect_split_free(argv, argv.size() - 1);
	close_fd(&cgi_data.pipe[1]);
	s_cgi	*cgi_ptr = new s_cgi(cgi_data);
	client.bindCgiSocket(srv, *cgi_ptr);
}

int read_file(std::string name, std::vector<char> &vect, int bytes);

static void		run_daemon(Server &srv, Client &client, t_cgi &cgi_data, argvVector &argv)
{
	ipPortCgiMap::iterator	cgi_exist;

	cgi_exist = srv.getIpPortCgiMap().find(client.getRequest().getHost());
	if (cgi_exist != srv.getIpPortCgiMap().end())
	{
		cgi_data = cgi_exist->second;
		if (srv.getAddrs()[cgi_data.poll_index[0]].events & POLLIN)
		{
			vect_split_free(argv, argv.size() - 1);
			std::cout << "goodbye\n";
				return ;
		}
		write(cgi_data.pipe[1], argv[1], std::strlen(argv[1]));
	}
	else
	{
		int		pipes[2][2];

		if (pipe(pipes[0]) != 0 || pipe(pipes[1]) != 0)
			return (std::cout << "run_script fatal error: pipe\n", (void)0);
		fcntl(cgi_data.pipe[0], FD_CLOEXEC);
		fcntl(cgi_data.pipe[1], FD_CLOEXEC);
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
			vect_split_free(argv, argv.size() - 1);
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
	s_cgi	*cgi_ptr = &cgi_exist->second;
	client.bindCgiSocket(srv, *cgi_ptr);
	vect_split_free(argv, argv.size() - 1);
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
