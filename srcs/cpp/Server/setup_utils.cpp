#include "../../hpp/Server.hpp"

dirent	*findUrlDirectory(std::string url);

//NOTE - crea una struct pollfd con l'fd del client, dato da accept()
struct pollfd	setupPollFd(int socket)
{
	struct pollfd s;

	s.fd = socket;
	s.events = POLLIN;
	s.revents = 0;
	return (s);
}

// NOTE - crea un socket listen per il server che vogliamo creare
struct pollfd	createServerSock(int port_n)
{
	struct sockaddr_in	address;
	int					server_fd;
	struct pollfd		srv;
	int					opt = 1;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		throw std::runtime_error("\033[31mSocket ha fallito.\033[0m");
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;// 7F000001 
	address.sin_port = htons(port_n);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) != 0)
	{
		close(server_fd);
		return (CONNECTION_FAIL);
	}
	if (listen(server_fd, MAX_CONNECTION) != 0)
	{
		close(server_fd);
		throw std::runtime_error("\033[31mIl server ha le orecchie tappate.\033[0m");
	}
	srv.fd = server_fd;
	srv.events = POLLIN;
	srv.revents = 0;
	return (srv);
}

std::string	fileToString(std::string filename)
{
	std::ifstream	fd(filename.c_str());
	std::string		file;

	if (fd.fail())
		abort();
	std::getline(fd, file, '\0');
	return (file);
}

