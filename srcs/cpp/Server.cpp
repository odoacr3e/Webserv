
#include "../hpp/Server.hpp"

Server::Server()
{
	// DBG_MSG("Server constructor called");
	int serverfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in	address;
	struct pollfd		srv;
	if (serverfd < 0)
		throw std::exception();
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080); //LINK: da cambiare e settare in base al config file
	if (bind(serverfd, (struct sockaddr*)&address, sizeof(address)) != 0)
		throw std::exception();
	srv.fd = serverfd;
	srv.events = POLLIN;
	this->_addrs.push_back(srv);
}

Server::~Server()
{
	// DBG_MSG("Server destructor called!");
}

struct pollfd&	setupPollFd(int client)
{
	struct pollfd s = {0};
	// s = (struct pollfd){0};
	s.fd = client;
	s.events = POLLIN;
	return (s);
}

void	Server::addSocket()
{
	int client = accept(this->_addrs[0].fd, NULL, NULL);
	if (client == -1)
		throw std::exception();
	std::cout << "connessione trovata, client: " << client << std::endl;
	this->_addrs.push_back(setupPollFd(client));
}

struct pollfd *Server::getAddrs(void)
{
	return (this->_addrs.data());
}

size_t	Server::getAddrSize(void) const
{
	return (this->_addrs.size());
}

void			Server::checkForConnection() //checkare tutti i socket per vedere se c'e stata una connessione di un client
{
	for (std::vector<struct pollfd>::iterator it = this->_addrs.begin() + 1; it != this->_addrs.end(); ++it)
	{
		if ()
	}
}