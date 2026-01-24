
#include "../hpp/Server.hpp"//joino su discord
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
	this->_addrs.push_back(setupPollFd(client));
	//creiamo oggetto client e lo aggiungiano alla std::map
	this->_clients[client] = new Client(client, this->_addrs.data()[index].fd);
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

std::string	createHtml(Client &client, const std::string &body, const std::string &type)
{
	std::ostringstream	response;
	std::string			http_codes_str[] = VALID_HTTP_STR;
	std::string			url = client.getRequest().getUrl().erase(0, 1);
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

	return (response.str());
}

static void	choose_file(Client &client, std::string &type, std::string fname, std::fstream &file, std::string url)
{
	type.erase(0, 1);
	type = "text/" + type;

	if (client.getRequest().getStatusCode() != 200)
	{
		file.open(("www/var/errors/standard/" + fname).c_str());
		std::cout << "www/var/errors/standard/" + fname << std::endl;
	}
	else if (client.getRequest().getRequestErrorBool())
	{
		file.open(("www/var/errors/dns/" + fname).c_str());
		std::cout << "www/var/errors/dns/" + fname << std::endl;
	}
	else
	{
		file.open(url.c_str());
		if (file.fail())
		{
			client.getRequest().fail(HTTP_CE_NOT_FOUND, url + " file not found!");
			file.open(("www/var/errors/standard/" + fname).c_str());
			std::cout << "www/var/errors/standard/" + fname << std::endl;
		}
		else
			std::cout << url << std::endl;
	}
}

// 
dirent	*test(std::string url)
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
		url.erase(0, 1);
		dir = opendir(url.c_str());
		if (!dir)
			return (NULL);
	}
	printf("dir %s aperta!\n", url.c_str());
	content = readdir(dir);
	if (!content)
	{
		closedir(dir);
		printf("dir %s chiusa!\n", url.c_str());
		dir = NULL;
		return (NULL);
	}
	printf("content name: %s, content type: %s\n", content->d_name, (content->d_type == 4) ? "folder" : "file");
	return (content);
}

/*
	createAutoIndex
	1)	statico
	2)	WHILE con listDirectories

	
	listDirectories
	1)	mainipolare var
	3)	tornare stringstream
*/

// std::string formatTimestamp(time_t timestamp) 
// {
//     struct tm timeinfo;

//     localtime_r(&timestamp, &timeinfo);

//     std::ostringstream oss;
//     oss << (timeinfo.tm_year + 1900) << "-"        // anni
//         << (timeinfo.tm_mon + 1) << "-"           // mesi 1-12
//         << timeinfo.tm_mday << " "
//         << timeinfo.tm_hour << ":"
//         << timeinfo.tm_min << ":"
//         << timeinfo.tm_sec;

//     return oss.str();
// }

// bool isLeap(int year) {
//     return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
// }

// std::string formatTime(time_t t) 
// {
//     const int seconds_per_day = 86400;
//     int days = t / seconds_per_day;
//     int rem_seconds = t % seconds_per_day;

//     int year = 1970;
//     while (true) {
//         int days_in_year = isLeap(year) ? 366 : 365;
//         if (days >= days_in_year) {
//             days -= days_in_year;
//             year++;
//         } else break;
//     }

//     int month_lengths[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
//     if (isLeap(year)) month_lengths[1] = 29;

//     int month = 0;
//     while (days >= month_lengths[month]) {
//         days -= month_lengths[month];
//         month++;
//     }

//     int day = days + 1; // day of month
//     int hour = rem_seconds / 3600;
//     int minute = (rem_seconds % 3600) / 60;
//     int second = rem_seconds % 60;

//     std::ostringstream oss;
//     oss << year << "-" 
//         << (month + 1) << "-" 
//         << day << " "
//         << hour << ":" 
//         << minute << ":" 
//         << second;
//     return oss.str();
// }


void listDirectories(std::string &body, dirent *cont)
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

void	createAutoindex(Client &client, std::string &body)
{
	std::ifstream file("www/var/autoindex/autoindex.html");
	std::string line;
	dirent		*content;

	test(client.getRequest().getUrl());
	while (std::getline(file, line))
	{
		line.push_back('\n');
		if (line.find("{PATH}")	!= std::string::npos)
			body += line.replace(line.find('{'), 6, client.getRequest().getUrl());
		else
			body += line;
		if (line.find("<tbody>") != std::string::npos)
			break ;
	}
	content = test(client.getRequest().getUrl());
	while (content)
	{
		listDirectories(body, content);
		content = test(client.getRequest().getUrl());
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

std::string	createResponse(Client &client) // create html va messo anche percorso per il file
{
	std::fstream	file;
	std::string		body;
	std::string		type(".html");
	std::string		url = client.getRequest().getUrl().erase(0, 1);

	std::cout << "STATUS CODE NOW: " << client.getRequest().getStatusCode() << "\n";
	std::cout << "TYPE NOW: " << client.getRequest().getUrl() << std::endl;
	std::cout << "ERROR NOW: " << client.getRequest().getRequestErrorBool() << std::endl;
	if (url.find_last_of('.') != std::string::npos)
		type = url.substr(url.find_last_of('.'));
	std::cout << (client.getRequest().getAutoIndexBool() == true ? "autoindex true\n" : "autoindex off\n");
	if (client.getRequest().getAutoIndexBool())
		createAutoindex(client, body);
	if (type == ".css")
		choose_file(client, type, "style.css", file, url);
	else if (type == ".html")
		choose_file(client, type, "index.html", file, url);
	else if (type == ".ico")
		choose_file(client, type, "favicon.ico", file, url);
	else if (client.getRequest().getRequestErrorBool() == true)
	{
		type = ".html";
		choose_file(client, type, "default.html", file, url);
	}
	else
		client.getRequest().fail(HTTP_CE_NOT_FOUND, "File extension not recognized!");
	if (body.empty())
		body = file_opener(file);
	return (createHtml(client, body, type));
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
			{
				//std::cout << (*this->_srvnamemap) << std::endl;
				// (void)buffer;
				// this->_clients[(*it).fd]->getRequest() = fileToString();
				Request	&request = this->_clients[(*it).fd]->getRequest();
				//std::cout << buffer << std::endl;
				//leggo la richiesta inviata dal client
				// if (requestParsing(request, fileToString("test_request")))
				if (requestParsing(request, buffer))
				{
					(*it).events = POLLOUT;
					//da settare status code corretto senza fare return
					std::cout << "errore\n";
					return ;
				}
				convertDnsToIp(request, request.getHost(), *this->_srvnamemap);
				std::cout << "pair: " << request.getHost() << "\n";
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
		}
		else if ((*it).fd != -1 && ((*it).revents & POLLOUT))
		{
			// Rispondo
			std::cout << " ----Sent message----" << std::endl;
			std::string	html = createResponse(*(this->_clients[(*it).fd]));
			send((*it).fd, html.c_str(), html.length(), 0);
			(*it).events = POLLIN;
		}
	}
}

void	convertDnsToIp(Request &request, IpPortPair &ipport, SrvNameMap &srvmap)
{
	if (std::isdigit(ipport.first[0]) != 0)
		return;
	std::cout << "------------DNS CONVERSION------------\n";
	std::cout << "Host CONVERT DNS: " << ipport.first << "," << ipport.second << std::endl;
	if (ipport.first == "localhost")
	{
		std::cout << "DNS: convert localhost ----> 127.0.0.1\n\n";
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
	std::cout << "Qui ci arrivo?" << std::endl << std::endl;
	request.setRequestErrorBool(true);
}

void			Server::printServerConfiguration(Conf &conf, SrvNameMap::iterator it) const
{
	(void)conf;
	return ;
	std::cout << std::endl << "\033[1;37m" << "Creating server " << this->_server_num + 1<< "\033[0m" << std::endl;
	std::cout << "Listening on -> \033[1;32m" << (*it).first.first << ":" << (*it).first.second << "\033[0m" << std::endl;
	std::cout << (*it).second;
}
