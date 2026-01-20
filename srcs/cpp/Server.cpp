
#include "../hpp/Server.hpp"
#include <fstream>

//crea una struct pollfd con l'fd del client, dato da accept()
static struct pollfd	setupPollFd(int client)
{
	struct pollfd s;

	s.fd = client;
	s.events = POLLIN;
	s.revents = 0;
	return (s);
}

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

Server::Server(Conf &conf)
{
	pollfd	port_connection;

	this->_server_num = 0;
	if (conf.getSrvNameMap().size() == 0)
		throw (std::runtime_error("\nSpecify at least one listen in conf file"));
	this->_srvnamemap = &conf.getSrvNameMap();
	for (SrvNameMap::iterator it = conf.getSrvNameMap().begin(); \
		it != conf.getSrvNameMap().end(); it++)
	{
		port_connection = createServerSock((*it).first.second);
		if (port_connection.fd != -1)
		{
			this->_server_data[port_connection.fd] = &(*it).second;
			this->_addrs.push_back(port_connection);
			printServerConfiguration(conf, it);
			this->_server_num++;
		}
		else
			std::cout << std::endl << "\033[1;31mCan't bind ip:port -> " << (*it).first.first << ":" << (*it).first.second << std::endl;
	}
	if (this->_server_num == 0)
		throw (std::runtime_error("\nNo server could be binded."));
}

Server::~Server()
{
	std::cout << "\033[32mserver destructor!\033[0m" << std::endl;
	if (this->_addrs.data()[0].fd != -1)
		close(this->_addrs.data()[0].fd);
	for (std::vector<struct pollfd>::iterator it = this->_addrs.begin() + this->_server_num; it != this->_addrs.end(); ++it)
	{
		close((*it).fd);
		delete this->_clients[(*it).fd];
		this->_clients.erase((*it).fd);
		it = this->_addrs.erase(it) - 1;
	}
}

//stampa finche non si blocca
// TODO - da aggiungere parametro su addSocket con il socket del server di riferimento
void	Server::addSocket(int index)
{
	int client = accept(this->_addrs.data()[index].fd, NULL, NULL);
	if (client == -1)
		throw std::runtime_error("\033[31mconnessione non accettata.\n\033[0m");
	std::cout << "connessione trovata, client: " << client << std::endl;
	this->_addrs.push_back(setupPollFd(client));
	//creiamo oggetto client e lo aggiungiano alla std::map
	this->_clients[client] = new Client(client, this->_addrs.data()[index].fd);
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

SrvNameMap		&Server::getSrvNameMap() const
{
	return (*this->_srvnamemap);
}

std::string	create_http(Client &client) // create http va messo anche percorso per il file
{
	std::string	html;
	std::fstream file;

	std::string	line;
	std::string	body;
	std::string	conttype("text/html");

	std::string	url = client.getRequest().getUrl();
	std::cout << "url: " << url << std::endl;
	if (url.length() > 4 && url.substr(url.length() - 4) == ".css")
	{
		conttype = "text/css";
		if (client.getRequest().getStatusCode() == 200)
			file.open("www/var/errors/default/default.css");
		else
			file.open("www/var/style.css");
	}
	else
	{
		file.open("www/var/index.html");
	}
	if (file.is_open())
	{
		while (std::getline(file, line))
			body += line + "\n";
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
	for (std::vector<struct pollfd>::iterator it = this->_addrs.begin() + this->_server_num; it != this->_addrs.end(); ++it)
	{
		if ((*it).fd != -1 && ((*it).revents & POLLIN))
		{
			char buffer[2048] = {0};
			int bytes = recv((*it).fd, buffer, sizeof(buffer) - 1, 0);
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
				Request	&request = this->_clients[(*it).fd]->getRequest();
				//leggo la richiesta inviata dal client
				if (requestParsing(request, buffer, *this->_srvnamemap))
				{
					(*it).events = POLLOUT;
					std::cout << "errore\n";
					return ;
				}
				convertDnsToIp(request.getHost(), *this->_srvnamemap);
				if ((*this->_srvnamemap).count(request.getHost()) == 0)
					return ((*it).events = POLLOUT, std::cout << "server not found\n", (void)0);
				std::cout << "\033[33m" << "RICHIESTA CLIENT GESTITA DA SERVER " << request.getHost() << "\033[0m" << std::endl;
				std::cout << "SERVER DI RIFERIMENTO: " << (*this->_srvnamemap)[request.getHost()] << std::endl;
				// una volta parsata la richiesta HTTP va fatta la risposta vedendo in base al client uale server 
				(*it).events = POLLOUT;
			}
		}
		else if ((*it).fd != -1 && ((*it).revents & POLLOUT))
		{
			// Rispondo
			std::cout << " ----Sent message----" << std::endl;
			std::string	html = create_http(this->_clients[(*it).fd]);
			send((*it).fd, html.c_str(), html.length(), 0);
			(*it).events = POLLIN;
		}
	}
}

/*
	ipPortPair

	doppio for
		se IP == serv_name && PORT == port 
*/

void	convertDnsToIp(IpPortPair &ipport, SrvNameMap &srvmap)
{
	if (std::isdigit(ipport.first[0]) != 0)
		return;
	std::cout << "------------DNS CONVERSION------------\n";
	if (ipport.first == "localhost")
	{
		std::cout << "DNS: convert localhost ----> 127.0.0.1\n";
		ipport.first = "127.0.0.1";
		return ;
	}
	for (SrvNameMap::iterator it = srvmap.begin(); it != srvmap.end(); it++)
	{
		for (std::vector<std::string>::iterator vit = (*it).second.server_names.begin(); \
			vit != (*it).second.server_names.end(); vit++)
		{
			if (ipport.first == *vit && ipport.second == (*it).first.second)
			{
				std::cout << "DNS: convert " << ipport.first;
				ipport.first = (*it).first.first;
				std::cout << " ----> " << ipport.first << "\n";
				std::cout << "---------------DNS CONVERSION----------------\n";
				return ;
			}
		}
	}
	std::cout << "				FAIL!					\n";
	std::cout << "------------DNS CONVERSION------------\n";
}

// Vecchio controllo sulla corrispondenza dns ip
// size_t 	endpoint;
	// 
	// if (key == "Host" && !isdigit(val[0]))
	// {//www.name.it:9090
		// endpoint = val.find_last_of(':');
		// if (endpoint == std::string::npos)
			// return (this->_header[key] = "", (void)0);
		// std::string name(val.substr(0, endpoint));
		// std::string s_port(val.substr(endpoint + 1));
		// int			port = std::atoi(s_port.c_str());
		// std::cout << "DNS IP SOLVING " << name << "\n";
		// std::cout << "DNS PORT SOLVING " << port << "\n";
		// for (SrvNameMap::iterator it = srv_names.begin(); it != srv_names.end(); it++)// Ma che caz?
		// {
			// for (std::vector<std::string>::iterator vit = (*it).second.server_names.begin(); \
			// vit != (*it).second.server_names.end(); vit++)
			// {
				// std::cout << "Checking vit " << *vit << "\n";
				// if (name == *vit && port == (*it).first.second)
				// {//KEY == HOST VALORE IP:PORTA
					// CONTROLLA ANCHE PORTA
					// std::cout << "Trovato il figlio di puttana\n";
					// this->_header[key] = (*it).first.first; //associo ip del server
					// this->_header[key].append(val.substr(endpoint)); // appendo la porta presa da val
					// std::cout << "adesso e " << this->_header[key] << "\n";
					// return ;
				// }
			// }
		// }
	// }


// Server -> 
/*
void	Server::printServerConfiguration(Conf &conf, SrvNameMap::iterator it) const
{
	(void)conf;
	std::cout << std::endl << "\033[1;37m" << "Creating server " << this->_server_num + 1<< "\033[0m" << std::endl;
	std::cout << "Listening on -> \033[1;33m" << (*it).first.first << ":" << (*it).first.second << "\033[0m" << std::endl;
	// std::cout << "Configuration\n{\n";
	std::cout << "\033[1;35m{\033[0m\n";
	std::cout << "\033[0m\033[1;35m    root ->\t\t\033[3;37m" << (*it).second.root << std::endl;
	std::cout << "\033[0m\033[1;35m    index ->\t\t\033[3;37m" << (*it).second.index << std::endl;
	std::cout << "\033[0m\033[1;35m    client_max_body ->\033[3;37m\t" << (*it).second.client_max_body_size << std::endl;
	std::cout << "\033[0m\033[1;35m    server names ->\033[3;37m\t";
	for (size_t i = 0; i < (*it).second.server_names.size(); i++)
	{
		if (i != 0)
			std::cout << "\t\t\t";
		std::cout << (*it).second.server_names[i] << std::endl;
	}
	std::cout << "\033[0m\033[1;35m    location ->\t\033[3;37m\t";
	for (std::map<std::string, t_conf_location>::iterator it_loc = (*it).second.location.begin(); \
		it_loc != (*it).second.location.end(); it_loc++)
	{
		if (it_loc != (*it).second.location.begin())
			std::cout << "\t\t\t";
		std::cout << (*it_loc).first << std::endl;
	}
	std::cout << "\n\033[0m\033[1;35m}\033[0m" << std::endl;
}*/

void			Server::printServerConfiguration(Conf &conf, SrvNameMap::iterator it) const
{
	(void)conf;
	std::cout << std::endl << "\033[1;37m" << "Creating server " << this->_server_num + 1<< "\033[0m" << std::endl;
	std::cout << "Listening on -> \033[1;32m" << (*it).first.first << ":" << (*it).first.second << "\033[0m" << std::endl;
	std::cout << (*it).second;
}
