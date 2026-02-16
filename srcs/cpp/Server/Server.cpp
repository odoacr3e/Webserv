
#include "../../hpp/Server.hpp"

#define SIZE_STR "B", "KB", "MB", "GB", "TB"
#define MSG_END_CONNECTION "======================================\nchiudo connessione {INDEX}\n======================================\n"

std::string				createHtml(Client &client, const std::string &body);
struct pollfd			createServerSock(int port_n);
struct pollfd			setupPollFd(int client);
std::string				fileToString(std::string filename);
dirent					*findUrlDirectory(std::string url);

// NOTE - aggiungiamo il socket del server al vector di server
Server::Server(Conf &conf, const char **env):_env(env)
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
			printServerConfiguration(it);
			this->_server_num++;
		}
		else
			std::cout << std::endl << "\033[1;31mCan't bind ip:port -> " << (*it).first.first << ":" << (*it).first.second << std::endl;
	}
	if (this->_server_num == 0)
		throw (std::runtime_error("\nNo server could be binded."));
	ft_ls("./", this->_protected_files);
}

Server::~Server()
{
	std::cout << "\033[32mserver destructor!\033[0m" << std::endl;
	this->suppressSocket();
}

void Server::suppressSocket()
{
	//SECTION - client disconnection
	for (std::vector<struct pollfd>::iterator it = this->_addrs.begin() + this->_server_num; it != this->_addrs.end(); ++it)
	{
		close((*it).fd);
		delete this->_clients[(*it).fd];
		this->_clients.erase((*it).fd);
		it = this->_addrs.erase(it) - 1;
	}
	//SECTION - server disconnection
	for (int i = 0; i != this->_server_num; i++)
		close(this->_addrs[i].fd);
	//SECTION - socket buffer clearup
	for (packetBuffer::iterator it = this->_packet_buffer.begin(); \
	it != this->_packet_buffer.end(); it++)
		delete [] (*it);
}

//TODO - refactoring: spostare funzioni poll, request, response file(?)
void	Server::checkForConnection() //checkare tutti i socket client per vedere se c'e stata una connessione
{
	for (std::vector<struct pollfd>::iterator it = this->_addrs.begin() + this->_server_num; it != this->_addrs.end(); ++it)
	{
		if ((*it).fd != -1 && ((*it).revents & POLLIN)) // revents & POLLIN -> pronto per leggere
		{
			char buffer[2048] = {0};
			// std::cout << "bytes left: " << this->_clients[(*it).fd]->getRequest().getBytesLeft() << "\n";
			int bytes = recv((*it).fd, buffer, sizeof(buffer) - 1, 0);
			print_file("REQUEST", buffer, bytes);
			if (bytes <= 0)
			{
				static int	n;
				//da mettere in una funzione a parte
				std::string msgEndCon(MSG_END_CONNECTION);
				find_and_replace(msgEndCon, "{INDEX}", n++);
				std::cout << msgEndCon << "\033[2J\033[H";
				print_file("REQUEST", msgEndCon);
				close((*it).fd);
				if (this->_clients[(*it).fd])
				{
					delete this->_clients[(*it).fd];
					this->_clients.erase((*it).fd);
					it = this->_addrs.erase(it) - 1;
				}
			}
			else
				processRequest(it, buffer, bytes);
		}
		else if ((*it).fd != -1 && ((*it).revents & POLLOUT)) // revents & POLLOUT -> pronto per ricevere
		{
			std::string	html = createResponse(*(this->_clients[(*it).fd]));
			// std::cout << "checkForConnection: response: " << html << std::endl;
			//std::cout << "checkForConnection: totalBinBody: " << std::endl;
			//for (size_t i = 0; i != this->_clients[(*it).fd]->getRequest().getBinBody().size();i++)
			//	std::cout << this->_clients[(*it).fd]->getRequest().getBinBody().data()[i];
			//std::cout << "\n";
			std::cout << "bodyfinal\n" << html << "\nENDRESP\n\n";
			send((*it).fd, html.c_str(), html.length(), 0);
			static int	n_resp;
			print_file("RESPONSE", html);
			std::string msgEndCon(MSG_END_CONNECTION);
			find_and_replace(msgEndCon, "{INDEX}", n_resp++);
			print_file("RESPONSE", msgEndCon);
			std::vector<char>	&contentData = this->_clients[(*it).fd]->getBuffer();
			if (this->_clients[(*it).fd]->sendContentBool() == true)
				send((*it).fd, contentData.data(), contentData.size(), 0);
			this->_clients[(*it).fd]->sendContentBool() = false;
			(*it).events = POLLIN;
		}
	}
}

void	Server::setupRequestEnvironment(Client &client)
{
	Request			&request = client.getRequest();
	t_conf_server	*srv;
	t_conf_location	*loc;
	unsigned char	allowed_methods;

	srv = &(*this->_srvnamemap)[request.getHost()];
	client.getSrvConf() = *srv;
	loc = request.findRightLocation(srv);
	if (loc)
		client.getLocConf() = *loc;
	request.findRightUrl(&(*this->_srvnamemap)[request.getHost()]);
	if (loc)
		allowed_methods = loc->mask_methods;
	else
		allowed_methods = srv->mask_methods;
	if ((allowed_methods & (1 << request.getMethodEnum())) == MASK_NO_METHODS)
		request.fail(HTTP_CE_METHOD_NOT_ALLOWED, "Ti puzzano i piedi (della zia del tuo ragazzo)");
}

void	Server::processRequest(std::vector<struct pollfd>::iterator &it, char *buffer, int bytes)
{
	Request	&request = this->_clients[(*it).fd]->getRequest();
	t_conf_server	srv;
	t_conf_location	loc;
	request.getRequestStream().str(buffer);
	request.getRequestStream().clear();
	request.getSockBytes() = bytes;

	if (request.getFirstRead() == true) // legge la prima volta
	{//TODO - refactoring: per favore splittiamo sti controlli
		request.getFirstRead() = false;
		if (requestParsing(*this->_clients[(*it).fd], buffer, bytes) != 0)//request
		{
			(*it).events = POLLOUT;
			// TODO - da settare status code corretto senza fare return ?????
			return ;
		}
		convertDnsToIp(request, request.getHost(), *this->_srvnamemap);// serverFinder
		if ((*this->_srvnamemap).count(request.getHost()) == 0)
		{
			(*it).events = POLLOUT;
			request.setRequestErrorBool(true);
			return ;
		}
		if ((size_t)(*this->_srvnamemap)[request.getHost()].client_max_body_size < request.getBodyLen())
		{//TODO - refactoring: bleah che schifo, poi gestire da conf
			if (!(request.getHeaderVal("Content-Type").find("multipart/form-data") != std::string::npos && request.getMethodEnum() == POST))
				request.fail(HTTP_CE_CONTENT_UNPROCESSABLE, "Declared max body size exceeded in current request (che scimmia che sei)");
		}
		this->setupRequestEnvironment(*this->_clients[(*it).fd]);
	}
	else
	{
		if (bodyHeaderParsing(request) == true)
		{
			request.getBinBody().insert(request.getBinBody().end(), request.getSockBuff(), request.getSockBuff() + request.getSockBytes());
			request.getBytesLeft() -= request.getSockBytes();
		}
	}
	if (request.getBytesLeft() == 0)
	{
		std::cout << "processRequest(): Sto andando in POLLOUT" << std::endl;
		(*it).events = POLLOUT;
		request.getFirstRead() = true;
	}
}

// NOTE - crea la risposta html da inviare al client tramite HTTP
std::string	Server::createResponse(Client &client) // create html va messo anche percorso per il file
{
	std::fstream	file;
	std::string		body;
	std::string		type("text/");
	std::string		url;

	url = client.getRequest().getUrl();
	if (url.find_last_of('.') != std::string::npos)
	{
		if (url.substr(url.find_last_of('.')) != ".html" && url.substr(url.find_last_of('.')) != ".css")
			type = "image/";
		type += url.substr(url.find_last_of('.')).erase(0, 1);
	}
	else
	{
		type += "html";
		// if (url.rbegin()[0] != '/')
			// std::cout << "passed a file in choose_file with no extension!" << std::endl;
	}
	if (client.getRequest().getAutoIndexBool() && valid_directory(url) && client.getRequest().getMethodEnum() != POST)
		createAutoindex(client, body);
	//else if (client.getRequest().getMethodEnum() != POST)
	else
		choose_file(client, file, url);
	client.getRequest().setBodyType(type);
	client.getBuffer().clear();
	runMethod(client, body, file);
	return (createHtml(client, body));
}

void	Server::runMethod(Client &client, std::string &resp_body, std::fstream &file)
{
	if (resp_body.empty() == false)
		return ;
	if (client.getRequest().getFailMsg().empty() == false)
	{
		resp_body = file_opener(file, "runMethod GET: Cannot open file");
		return ;
	}
	if (client.getRequest().getRunScriptBool() == true)
		run_script(*this, client, resp_body);
	switch (client.getRequest().getMethodEnum())
	{
		case GET:
			if (client.getRequest().getRunScriptBool() == true)//FIXME - forzo per debug
				break ;
			std::cout << "runMethod(): reading file..\n";
			client.sendContentBool() = true;
			//std::string file = client.getRequest().getUrl();
			read_file(file, client.getBuffer());
			client.getBuffer().push_back('\n');
			client.getBuffer().push_back('\n');
			break ;
		case DELETE:
			this->deleteMethod(client, resp_body, &file);
			break ;
		case POST:
			//parseData
			this->postMethod(client, resp_body, &file);
			//se script lancia lo script
			//funzione che gestisce POST
			break ;
		case HEAD:
			;//funzione che gestisce HEAD
			break ;
		case METH_NUM:
			break ;
	}
}

void	Server::choose_file(Client &client, std::fstream &file, std::string url)
{
	std::string	fname;

	if (client.getRequest().getDnsErrorBool())
		file.open("www/var/errors/dns/index.html");
	else if (client.getRequest().getStatusCode() != 200)
	{
		fname = checkErrorPages(client.getRequest());
		file.open((fname).c_str());
	}
	else if (client.getRequest().getRunScriptBool() == false)
	{
		file.open(url.c_str());
		if (file.fail())
		{
			client.getRequest().fail(HTTP_CE_NOT_FOUND, url + ": File not found!");
			fname = checkErrorPages(client.getRequest());
			file.open((fname).c_str());
		}
	}
}

// NOTE - crea un body per autoindex delle cartelle, utilizza dirent * e findUrlDirectory()
//TODO - refactoring file a parte queste funzioni
void	Server::createAutoindex(Client &client, std::string &resp_body)
{
	std::ifstream	file("www/var/autoindex/autoindex.html");
	std::string		line;
	dirent			*content;
	std::string		url;

	url = client.getRequest().getUrl();
	while (std::getline(file, line))
	{
		line.push_back('\n');
		find_and_replace(line, "{PATH}", client.getRequest().getUrlOriginal());
		resp_body += line;
		if (line.find("<tbody>") != std::string::npos)
			break ;
	}
	content = findUrlDirectory(url);
	while (content)
	{
		std::string dname = content->d_name;
		if (dname[0] != '.')
			listDirectoriesAutoIndex(resp_body, url, content);//FIXME - da vedere qui per stat
		content = findUrlDirectory(url);
	}
	while (std::getline(file, line))
		if (line.find("</tbody>") != std::string::npos)
			break ;
	resp_body += line;
	while (std::getline(file, line))
	{
		find_and_replace(line, "{SERVER_NAME}", "3 UOMINI E 1 WEBSERVER");
		resp_body += line;
	}
}

// NOTE - crea html come body per la risposta da inviare al client
std::string	createHtml(Client &client, const std::string &body)
{
	std::ostringstream	response;
	std::string			http_codes_str[] = VALID_HTTP_STR;
	std::string			url = client.getRequest().getUrl();
	int 				status = client.getRequest().getStatusCode();

	response << "HTTP/1.1 "
	         << status << " "
	         << http_codes_str[checkValidCode(status)] << "\r\n";
	response << "Content-Type: " << client.getRequest().getBodyType() << "\r\n";
	if (client.sendContentBool() == true)
	{
		response << "Content-Length: " << client.getBuffer().size();
		response << "\r\n\r\n";
	}
	else
	{
		response << "Content-Length: " << body.size() << "\r\n\r\n";
		response << body << "\n\n";
	}
	std::cout << "createHtml() URL: " << url << std::endl;
	return (response.str());
}

//FIXME - da inserire append root/alias in config file
std::string	Server::checkErrorPages(Request &request)
{
	std::ifstream	file;
	s_conf_server 	*server = &(*this->_srvnamemap)[request.getHost()];
	s_conf_location	*loc;
	int				status_code = request.getStatusCode();
	std::string 	url = request.getUrl();

	if (server->location.count(url) == 0) // check se non ci sono location
	{
		if (server->err_pages.count(status_code) > 0) // check su server se ci sono error pages adeguate
		{
			file.open((server->root + server->err_pages[status_code]).c_str());
			if (file.fail() == true)
				return (url_rooting("/errors/default.html", *server));
			return (url_rooting(server->err_pages[status_code], *server));
		}
	}
	else if (server->location[url].err_pages.count(status_code) > 0) // controllo se location ha l'error page richiesta
	{
		loc = &server->location[url];
		file.open((server->root + server->location[url].err_pages[status_code]).c_str());
		std::cout << "Location prova ad aprire: " << loc->root + server->location[url].err_pages[status_code] << std::endl;
		if (file.fail() == true)
			return (url_rooting("/errors/default.html", *loc));
		return (url_rooting(server->location[url].err_pages[status_code], *loc));
	}
	return (server->root + "/errors/default.html"); // return di default
}


std::string	calculateSize(size_t s)
{
	int 	len = 0;
	size_t	tmp = s;
	static std::string	size_str[] = {SIZE_STR, ""}; 

	while (tmp > 0)
	{
		tmp /= 10;
		len++;
	}
	std::string type = size_str[(len - 1) / 3];
	while (len > 3)
	{
		s /= 1000;
		len -= 3;
	}
	return (ft_to_string(s) + " " + type);
}

// NOTE - prende da un file statico l'html e cambia parametri variabili che servono per il body html
void Server::listDirectoriesAutoIndex(std::string &body, std::string &url, dirent *cont)
{
	std::ifstream var("www/var/autoindex/var.html");
	std::string	path;
	std::string line;
	std::string	s_cont;
	struct stat	info;

	s_cont = (std::string)cont->d_name + (cont->d_type == 8 ? "" : "/");
	std::memset(&info, 0, sizeof(struct stat));
	path = url + '/' + cont->d_name;
	stat(path.c_str(), &info);
	if (var.fail())
	{
		std::cout << "Could not open html file!\n";
		return ;
	}
	while(std::getline(var, line))
	{
		line.append("\n");
		find_and_replace(line, "href=\"", "href=\"" + s_cont);
		find_and_replace(line, "{NAME}", s_cont);
		find_and_replace(line, "{SIZE}", calculateSize(info.st_size));
		find_and_replace(line, "{MODIFY}", std::ctime(&info.st_mtim.tv_sec));
		body += line;
	}
}

void	convertDnsToIp(Request &request, IpPortPair &ipport, SrvNameMap &srvmap)
{
	if (std::isdigit(ipport.first[0]) != 0)
		return;

	if (ipport.first == "localhost")
	{
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
				ipport.first = (*it).first.first;
				std::cout << " ----> " << ipport.first << "\n";
				return ;
			}
		}
	}
	request.setRequestErrorBool(true);
}

void	Server::printServerConfiguration(SrvNameMap::iterator it) const
{
	if (!SERVER)
		return ;
	std::cout << std::endl << "\033[1;37m" << "Creating server " << this->_server_num + 1<< "\033[0m" << std::endl;
	std::cout << "Listening on -> \033[1;32m" << (*it).first.first << ":" << (*it).first.second << "\033[0m" << std::endl;
	//NOTE - questa linea stampa tutto l'universo
	//std::cout << (*it).second;
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
