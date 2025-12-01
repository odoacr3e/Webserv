
#include "../hpp/Server.hpp"

static struct pollfd	setupPollFd(int client);

Server::Server()
{
	struct sockaddr_in	address;
	struct pollfd		srv;

	std::cout << "\033[32mserver constructor!\033[0m" << std::endl;
	// DBG_MSG("Server constructor called");
	this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_server_fd < 0)
		throw std::runtime_error("\033[31mSocket ha fallito.\033[0m");
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080); //LINK: da cambiare e settare in base al config file
	if (bind(_server_fd, (struct sockaddr*)&address, sizeof(address)) != 0)
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
	// s = (struct pollfd){0};
	s.fd = client;
	s.events = POLLIN;
	s.revents = 0;
	return (s);
}

//stampa finche non si blocca
void	Server::addSocket()
{
	std::cout << "server fd: " << this->_server_fd << ", addr[0].fd: " << this->_addrs[0].fd << std::endl;
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
	// std::cout << html << std::endl;
	return (html + "\n");
}

/*
		for (int i = 1; i <= 5; i++)
		{
			std::cout << "revents Fd[" << i << "]:" << fds[i].revents << std::endl;
			if (fds[i].fd != -1 && (fds[i].revents & (POLLIN) || \
			(fds[i].revents & (POLLOUT)) || (fds[i].revents & (POLLERR))))
			{
				std::cout << "Entrato:\t" << fds[i].fd << std::endl;
				char buffer[1024];
				int bytes = read(fds[i].fd, buffer, sizeof(buffer));
				if (bytes <= 0)
				{
					printf("Client %d disconnesso\n", fds[i].fd);
					close(fds[i].fd);
					fds[i].fd = -1;
				}
				else
				{
					// Rispondo
					std::cout << " ----Sent message----" << std::endl;
					std::string	html = create_html("mega gay");
					send(fds[i].fd, html.c_str(), html.length(), 0);
					close(fds[i].fd);
					fds[i].fd = -1;
				}
			}
*/
void	Server::checkForConnection() //checkare tutti i socket per vedere se c'e stata una connessione di un client
{
	// questa cosa del read va provata da browser facendo una richiesta e poi chiudendo la pagina
	//stampiamo quando un client viene disconnesso e vediamo se il server la stampa
	//1024 credo sia per la struttura dell'http che richiede un minimo di caratteri (ipotesi)
	for (std::vector<struct pollfd>::iterator it = this->_addrs.begin() + 1; it != this->_addrs.end(); ++it)
	{
		// std::cout << "ARRIVA 4" << std::endl;
		if ((*it).fd != -1 && ((*it).revents & (POLLIN) || ((*it).revents & (POLLOUT)))) //POLLERR mi sa va gestito a parte, per ora non mettia
		{
			//&data[i] == it
			std::cout << "Entrato:\t" << (*it).fd << std::endl;
			char buffer[256];
			int bytes = read((*it).fd, buffer, sizeof(buffer));
			if (bytes <= 0)
			{
				printf("Client %d disconnesso\n", (*it).fd);
				close((*it).fd); //si, va pullato
				// this->_server_fd
				(*it).fd = -1;
			}
			else
			{
				// Rispondo
				std::cout << " ----Sent message----" << std::endl;
				std::string	html = create_html("mega gay");
				send((*it).fd, html.c_str(), html.length(), 0);
				close((*it).fd);
				(*it).fd = -1;
			}
		}
	}
}
//ho il terminale buggato cmq mi e rimasto con i printf sullo schermo
//raga io vado a casa
//se ci siete ancora mi ricollego
//a despues