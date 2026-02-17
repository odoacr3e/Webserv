#include "../../hpp/Server.hpp"

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

	print_file("REQUEST", buffer, bytes);
	if (request.getFirstRead() == true) // legge la prima volta
	{
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
