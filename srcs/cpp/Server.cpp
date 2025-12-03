
#include "../hpp/Server.hpp"

static struct pollfd	setupPollFd(int client);

Server::Server()
{
	struct sockaddr_in	address;
	struct pollfd		srv;

	std::cout << "\033[32mserver constructor!\033[0m" << std::endl;
	this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_server_fd < 0)
		throw std::runtime_error("\033[31mSocket ha fallito.\033[0m");
	address.sin_family = AF_INET;
	//TODO: da aggiungere indirizzo ip in base a quelli richiesti dal config file
	address.sin_addr.s_addr = INADDR_ANY;
	//TODO: da cambiare e settare in base al config file le porte in ascolto
	address.sin_port = htons(8080);
	if (bind(this->_server_fd, (struct sockaddr*)&address, sizeof(address)) != 0)
		throw std::runtime_error("\033[31mBind ha fallito.\033[0m");
	if (listen(_server_fd, MAX_CONNECTION) != 0)
		throw std::runtime_error("\033[31mIl server ha le orecchie tappate.\033[0m");
	srv.fd = _server_fd;
	srv.events = POLLIN;
	srv.revents = 0;
	this->_addrs.push_back(srv);
	std::cout << "\033[32mTutto bene col costruttore!\033[0m" << std::endl;
}

Server::~Server()
{
	std::cout << "\033[32mserver destructor!\033[0m" << std::endl;
	if (this->_server_fd != -1)
		close(this->_server_fd);
	for (size_t i = 0; i < this->_addrs.size(); i++)
		if (this->_addrs.data()[i].fd != -1)
			close(this->_addrs.data()[i].fd);
}

static struct pollfd	setupPollFd(int client)
{
	struct pollfd s;
	s.fd = client;
	s.events = POLLIN;
	s.revents = 0;
	return (s);
}

//stampa finche non si blocca
void	Server::addSocket()
{
	int client = accept(this->_server_fd, NULL, NULL);
	if (client == -1)
		throw std::runtime_error("\033[31mconnessione non accettata.\n\033[0m");
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

std::string	create_html(std::string body)
{
	std::string	html;

	html += "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
	html += std::to_string(body.length() + 1);
	html += "\r\n\r\n";
	html += body + "\n";
	return (html + "\n");
}

void	Server::checkForConnection() //checkare tutti i socket client per vedere se c'e stata una connessione
{
	for (std::vector<struct pollfd>::iterator it = this->_addrs.begin() + 1; it != this->_addrs.end(); ++it)
	{
		if ((*it).fd != -1 && ((*it).revents & POLLIN))
		{
			char buffer[1024];
			int bytes = recv((*it).fd, buffer, sizeof(buffer), 0);
			if (bytes <= 0)
			{
				std::cout << "chiudo" << std::endl;
				close((*it).fd);
				(*it).fd = -1;
			}
			else
			{
				std::cout << buffer << std::endl; //leggo la richiesta inviata dal client
				std::cout << "POLLOUT CLIENT" << std::endl;
				(*it).events = POLLOUT;
			}
		}
		else if ((*it).fd != -1 && ((*it).revents & POLLOUT))
		{
			// Rispondo
			std::cout << " ----Sent message----" << std::endl;
			std::string	html = create_html("mega gay");
			send((*it).fd, html.c_str(), html.length(), 0);
			(*it).events = POLLIN;
		}
	}
}
