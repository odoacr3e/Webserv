
#include "../../hpp/Server.hpp"

std::string				createHtml(Client &client, const std::string &body, const std::string &type);
void 					listDirectoriesAutoIndex(std::string &body, dirent *cont);
static dirent			*findUrlDirectory(std::string url);
struct pollfd			createServerSock(int port_n);
struct pollfd			setupPollFd(int client);

// NOTE - aggiungiamo il socket del server al vector di server
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
				static int	n;
				//da mettere in una funzione a parte
				std::cout << "chiudo connessione " << n++ << std::endl;
				std::cout << "======================================" << std::endl;
				std::cout << "\033[2J\033[H";
				close((*it).fd);
				if (this->_clients[(*it).fd])
				{
					delete this->_clients[(*it).fd];
					this->_clients.erase((*it).fd);
					it = this->_addrs.erase(it) - 1;
				}
			}
			else
				processRequest(it, buffer);
		}
		else if ((*it).fd != -1 && ((*it).revents & POLLOUT))
		{
			std::string	html = createResponse(*(this->_clients[(*it).fd]));
			send((*it).fd, html.c_str(), html.length(), 0);
			(*it).events = POLLIN;
		}
	}
}

void	Server::processRequest(std::vector<struct pollfd>::iterator it, char *buffer)
{
	Request	&request = this->_clients[(*it).fd]->getRequest();
	if (requestParsing(request, buffer))
	{
		(*it).events = POLLOUT;
		// TODO - da settare status code corretto senza fare return
		std::cout << "errore nel parsing request\n";
		return ;
	}
	convertDnsToIp(request, request.getHost(), *this->_srvnamemap);
	std::cout << "Server nums: " << (*this->_srvnamemap).count(request.getHost()) << std::endl;
	if ((*this->_srvnamemap).count(request.getHost()) == 0)//condition
	{
		request.setRequestErrorBool(true);
		request.setStatusCode(HTTP_OK);
		std::cout << "SERVER NOT FOUND\n" << std::endl;
	}
	else
	{
		if ((size_t)(*this->_srvnamemap)[request.getHost()].client_max_body_size < request.getBodyLen())
			request.fail(HTTP_CE_CONTENT_UNPROCESSABLE, "Declared max body size exceeded in current request (che scimmia che sei)");
		request.findRightPath(&(*this->_srvnamemap)[request.getHost()]);
	}
	(*it).events = POLLOUT;
}

// NOTE - crea la risposta html da inviare al client tramite HTTP
std::string	Server::createResponse(Client &client) // create html va messo anche percorso per il file
{
	std::fstream	file;
	std::string		body;
	std::string		type("text/");
	std::string		url = client.getRequest().getUrl();

	//TODO Va fatta una funzione che cambi il content-type in base al tipo di file
	if (url.find_last_of('.') != std::string::npos)
	{
		if (url.substr(url.find_last_of('.')) == ".ico")
			type = "image/";
		type += url.substr(url.find_last_of('.')).erase(0, 1);
	}
	else
	{
		type += "html";
		if (url.rbegin()[0] != '/')
			std::cout << "passed a file in choose_file with no extension!" << std::endl;
	}
		std::cout << (client.getRequest().getAutoIndexBool() == true ? "autoindex true\n" : "autoindex off\n");
	if (client.getRequest().getAutoIndexBool())
	{
		createAutoindex(client, body);
	}
	else
	{
		std::cout << "Url della request: " << url << std::endl;
		choose_file(client, file, url);
	}
//	else
//		client.getRequest().fail(HTTP_CE_NOT_FOUND, "File extension not recognized!");
	std::cout << "URL after: " << url << std::endl;
	if (body.empty())
		body = file_opener(file);
	return (createHtml(client, body, type));
}

void	Server::choose_file(Client &client, std::fstream &file, std::string url)
{
	std::string	fname;

	if (client.getRequest().getRequestErrorBool())
		file.open("www/var/errors/dns/index.html");
	else if (client.getRequest().getStatusCode() != 200)
	{
		fname = checkErrorPages(client.getRequest());
		file.open((fname).c_str());
		std::cout << "NAME ERROR: " << fname << std::endl;
		// std::cout << "fname not 200: " << fname << std::endl;
	}
	else
	{
		file.open(url.c_str());
		if (file.fail())
		{
			client.getRequest().fail(HTTP_CE_NOT_FOUND, url + " file not found!");
			fname = checkErrorPages(client.getRequest());
			file.open((fname).c_str());
			std::cout << "NAME ERROR: " << fname << std::endl;
		}
	}
}

// NOTE - crea un body per autoindex delle cartelle, utilizza dirent * e findUrlDirectory()
void	Server::createAutoindex(Client &client, std::string &body)
{
	std::ifstream	file("www/var/autoindex/autoindex.html");
	std::string		line;
	dirent			*content;
	std::string		url;

	url = client.getRequest().getUrl();
	// TODO - aggiungere parametro alla richiesta (basic_url) che mantenga l'url della richiesta nudo e crudo senza modifiche da displayare nell'autoindex e varie pagine
	findUrlDirectory(url);
	while (std::getline(file, line))
	{
		line.push_back('\n');
		if (line.find("{PATH}")	!= std::string::npos)
			body += line.replace(line.find('{'), 6, url);
		else
			body += line;
		if (line.find("<tbody>") != std::string::npos)
			break ;
	}
	content = findUrlDirectory(url);
	while (content)
	{
		listDirectoriesAutoIndex(body, content);
		content = findUrlDirectory(url); 
	}
	while (std::getline(file, line))
		if (line.find("</tbody>") != std::string::npos)
			break ;
	body += line;
	while (std::getline(file, line))
	{
		if (line.find("{SERVER_NAME}")	!= std::string::npos)
			body += line.replace(line.find('{'), 13, "3 UOMINI E 1 WEBSERVER");
		else
			body += line;
	}
}

// NOTE - crea html come body per la risposta da inviare al client
std::string	createHtml(Client &client, const std::string &body, const std::string &type)
{
	std::ostringstream	response;
	std::string			http_codes_str[] = VALID_HTTP_STR;
	std::string			url = client.getRequest().getUrl();
	int 				status = client.getRequest().getStatusCode();

	response << "HTTP/1.1 "
	         << status << " "
	         << http_codes_str[checkValidCode(status)] << "\r\n";
	response << "Content-Type: " << type << "\r\n";
	response << "Content-Length: " << body.size() << "\r\n\r\n";
	response << body << "\n\n";

	std::cout << "CREATE RESPONSE ERROR: " << (client.getRequest().getRequestErrorBool() == true ? "true" : "false") << std::endl;
	std::cout << "CREATE RESPONSE STATUS: " << client.getRequest().getStatusCode() << std::endl;
	std::cout << "URL: " << url << std::endl;

	// std::cout << response.str() << std::endl;
	return (response.str());
}

//FIXME - da inserire append root/alias in config file
std::string	Server::checkErrorPages(Request &request)
{
	std::ifstream	file;
	s_conf_server 	*server = &(*this->_srvnamemap)[request.getHost()];
	s_conf_location	*loc; //?
	int				status_code = request.getStatusCode();
	std::string 	url = request.getUrl();

	if (server->location.count(url) == 0) // check se non ci sono location
	{
		if (server->err_pages.count(status_code) > 0) // check su server se ci sono error pages adeguate
		{
			file.open((server->root + server->err_pages[status_code]).c_str());
			std::cout << "Status code error page: " << status_code << std::endl; 
			std::cout << "Server prova ad aprire: " << server->root + server->err_pages[status_code] << std::endl;
			if (file.fail() == true)
				return (app_root_alias("/errors/default.html", *server));
				// return (server->root + "/errors/default.html");
			return (app_root_alias(server->err_pages[status_code], *server));
			// return (server->root + server->err_pages[status_code]); // ritorni error page server
		}
	}
	else if (server->location[url].err_pages.count(status_code) > 0) // controllo se location ha l'error page richiesta
	{
		loc = &server->location[url];
		file.open((server->root + server->location[url].err_pages[status_code]).c_str());
		std::cout << "Location prova ad aprire: " << loc->root + server->location[url].err_pages[status_code] << std::endl;
		if (file.fail() == true)
			return (app_root_alias("/errors/default.html", *loc));
		return (app_root_alias(server->location[url].err_pages[status_code], *loc));
	}
	return (server->root + "/errors/default.html"); // return di default
}

// NOTE - cerca directory dentro l'URL passato come parametro, viene chiamata come gnl e ad ogni ritorno ritorna la cartella successiva
static dirent	*findUrlDirectory(std::string url)
{
	static DIR	*dir;
	dirent		*content;

	if (url.empty())
	{
		if (dir)
			closedir(dir);
		dir = NULL;
		return (NULL);
	}
	if (!dir)
	{
		dir = opendir(url.c_str());
		if (!dir)
			return (NULL);
	}
	content = readdir(dir);
	if (!content)
	{
		closedir(dir);
		dir = NULL;
		return (NULL);
	}
	return (content);
}

// NOTE - prende da un file statico l'html e cambia parametri variabili che servono per il body html
void listDirectoriesAutoIndex(std::string &body, dirent *cont)
{
	std::ifstream var("www/var/autoindex/var.html");
	std::string line;
	std::string	s_cont;
	struct stat	info;

	s_cont = (std::string)cont->d_name + (cont->d_type == 8 ? "" : "/");
	stat(cont->d_name, &info);
	if (var.fail())
	{
		std::cout << "Could not open html file!\n";
		return ;
	}
	while(std::getline(var, line))
	{
		line.append("\n");
		size_t pos = line.find("href=\"");
		if (pos != std::string::npos)
			line.replace(pos, 6, "href=\"" + s_cont);
		pos = line.find("{NAME}");
		if (pos != std::string::npos)
			line.replace(pos, 6, s_cont);
		pos = line.find("{SIZE}");
		if (pos != std::string::npos)
			line.replace(pos, 6, ft_to_string(info.st_size) + " byte");
		pos = line.find("{MODIFY}");
		if (pos != std::string::npos)
			line.replace(pos, 8, ft_to_string(info.st_mtim.tv_sec));
		body += line;
	}
}

void	convertDnsToIp(Request &request, IpPortPair &ipport, SrvNameMap &srvmap)
{
	if (std::isdigit(ipport.first[0]) != 0)
		return;
	// std::cout << "------------DNS CONVERSION------------\n";
	// std::cout << "Host CONVERT DNS: " << ipport.first << "," << ipport.second << std::endl;
	if (ipport.first == "localhost")
	{
		// std::cout << "DNS: convert localhost ----> 127.0.0.1\n\n";
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
				// std::cout << "DNS: convert " << ipport.first;
				ipport.first = (*it).first.first;
				std::cout << " ----> " << ipport.first << "\n";
				// std::cout << "---------------DNS CONVERSION----------------\n";
				return ;
			}
		}
	}
	// std::cout << "Qui ci arrivo?" << std::endl << std::endl;
	request.setRequestErrorBool(true);
}

void	Server::printServerConfiguration(Conf &conf, SrvNameMap::iterator it) const
{
	(void)conf;
	return ;
	std::cout << std::endl << "\033[1;37m" << "Creating server " << this->_server_num + 1<< "\033[0m" << std::endl;
	std::cout << "Listening on -> \033[1;32m" << (*it).first.first << ":" << (*it).first.second << "\033[0m" << std::endl;
	std::cout << (*it).second;
}



/* // NOTE - crea un socket listen per il server che vogliamo creare
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
} */

/* std::string	fileToString(std::string filename)
{
	std::ifstream	fd(filename.c_str());
	std::string		file;

	if (fd.fail())
		abort();
	std::getline(fd, file, '\0');
	return (file);
} */

// NOTE - creiamo oggetto client e lo aggiungiano alla mappa di puntatori client 
void	Server::addSocket(int index)
{
	int client = accept(this->_addrs.data()[index].fd, NULL, NULL);
	if (client == -1)
		throw std::runtime_error("\033[31mconnessione non accettata.\n\033[0m");
	this->_addrs.push_back(setupPollFd(client));
	this->_clients[client] = new Client(client, this->_addrs.data()[index].fd);
}