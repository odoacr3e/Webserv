
#include "../hpp/Server.hpp"
#include <fstream>

static struct pollfd	setupPollFd(int client);

/*
	//NOTE - ipotesi su come gestire più porte

	1)	Per ogni porta dobbiamo fare socket + bind + listen;
	2)	Registriamo tutti gli fd in un SINGOLO vector (_addrs);
	3)	Il vector deve avere questa struttura:
		Porte ascoltate: 8080, 8081, 8082
		Fd delle porte:		3,    4,    5
		vector _addrs
			_addrs[0] = 3; ---->	Listen 8080
			_addrs[1] = 4; ---->	Listen 8081
			_addrs[2] = 5; ---->	Listen 8082
			_addrs[3] = 6; ---->	Client A
			_addrs[4] = 7; ---->	Client B
	
	4)	Fare un solo poll, passandogli il vector.
		A poll frega un cazzo di dove gli arriva la roba.
*/
static struct pollfd	createServerSock(int port_n) //successivamente prendera una reference a un oggetto Config con tutti i parametri passati dal config file
{
	struct sockaddr_in	address;
	int					server_fd;
	struct pollfd		srv;
	int					opt = 1;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		throw std::runtime_error("\033[31mSocket ha fallito.\033[0m");
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port_n);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) != 0)
	{
		close(server_fd);
		throw std::runtime_error("\033[31mBind ha fallito.\033[0m");
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

/*
	server {listen 80 81;}
	server {listen 82;}
	server {}	->mettere porta 80 (predefinita)

	//FIXME - gestione blocchi server config
	costruttore server deve prendere come argomento:
	@std::vector<t_conf_server>:	ogni elemento è un blocco server nel conf

	Ogni blocco server contiene una o più porte in ascolto.
	Dobbiamo fare due cicli for:
	1)	per ogni elemento di std::vector<t_conf_server>:
	{
		2)	per ogni porta su cui l'elemento ascolta:
		{
			this->_addrs.push_back(createServerSock(PORT))
		}
		collegare tutti gli addrs al t_conf_server corrente
}
*/
Server::Server()
{
	this->_server_num = 0;
	std::cout << "\033[32mserver constructor!\033[0m" << std::endl;
	//for
	this->_addrs.push_back(createServerSock(DEFAULT_PORT));
	// _ROBBA[&(*(_addrs.rbegin()))] = 1;
	std::cout << "\033[32mTutto bene col costruttore!\033[0m" << std::endl;
}

Server::~Server()
{
	std::cout << "\033[32mserver destructor!\033[0m" << std::endl;
	// for (size_t i = 0; i < this->_addrs.size(); i++)
	// 	if (this->_addrs.data()[i].fd != -1)
	// 		close(this->_addrs.data()[i].fd);
	if (this->_addrs.data()[0].fd != -1)
		close(this->_addrs.data()[0].fd);
	for (std::vector<struct pollfd>::iterator it = this->_addrs.begin() + 1; it != this->_addrs.end(); ++it)
	{
		close((*it).fd);
		delete this->_clients[(*it).fd];
		this->_clients.erase((*it).fd);
		it = this->_addrs.erase(it) - 1;
	}
}

//crea una struct pollfd con l'fd del client, dato da accept()
static struct pollfd	setupPollFd(int client)
{
	struct pollfd s;

	s.fd = client;
	s.events = POLLIN;
	s.revents = 0;
	return (s);
}

//stampa finche non si blocca
// TODO - da aggiungere parametro su addSocket con il socket del server di riferimento
void	Server::addSocket()
{
	int client = accept(this->_addrs.data()[0].fd, NULL, NULL);
	if (client == -1)
		throw std::runtime_error("\033[31mconnessione non accettata.\n\033[0m");
	std::cout << "connessione trovata, client: " << client << std::endl;
	this->_addrs.push_back(setupPollFd(client));
	//creiamo oggetto client e lo aggiungiano alla std::map
	this->_clients[client] = new Client(client, this->_addrs.data()[0].fd);
	std::cout << &this->_clients[client] << std::endl;
}

struct pollfd *Server::getAddrs(void)
{
	return (this->_addrs.data());
}

size_t	Server::getAddrSize(void) const
{
	return (this->_addrs.size());
}

int	Server::getServerNum() const
{
	return (this->_server_num);
}

std::string	create_http(std::string url)
{
	std::string	html;
	std::fstream file;
	std::string	line;
	std::string	body;
	std::string	conttype("text/html");

	std::cout << "url: " << url << std::endl;
	if (url.length() > 4 && url.substr(url.length() - 4) == ".css")
	{
		file.open("www/var/style.css");
		conttype = "text/css";
	}
	else
		file.open("www/var/index.html");
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			body += line + "\n";
		}
		file.close();
	}
	html += "HTTP/1.1 200 OK\r\n";
	html += "Content-Type: " + conttype + "\r\n";
	html += "Content-Length: ";
	html += ft_to_string(body.length() + 1);
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
			char buffer[1024] = {0};
			int bytes = recv((*it).fd, buffer, sizeof(buffer), 0);
			if (bytes <= 0)
			{
				//da mettere in una funzione a parte
				std::cout << "chiudo" << std::endl;
				close((*it).fd);
				if (this->_clients[(*it).fd])
				{
					delete this->_clients[(*it).fd];
					this->_clients.erase((*it).fd);
					it = this->_addrs.erase(it) - 1;
				}
			}
			else
			{
				//leggo la richiesta inviata dal client
				std::cout << "\033[33m" << "RICHIESTA CLIENT" << "\033[0m" << std::endl;
				requestParsing(this->_clients[(*it).fd]->getRequest(), buffer);
				// una volta parsata la richiesta HTTP va fatta la risposta vedendo in base al client uale server 
				//this->_ROBBA[&this->_addrs[(*it).fd]]
				(*it).events = POLLOUT;
			}
		}
		else if ((*it).fd != -1 && ((*it).revents & POLLOUT))
		{
			// Rispondo
			std::cout << " ----Sent message----" << std::endl;
			std::string	html = create_http(this->_clients[(*it).fd]->getRequest().getUrl());
			send((*it).fd, html.c_str(), html.length(), 0);
			(*it).events = POLLIN;
		}
	}
}

// Server -> 
