#include "../../hpp/Server.hpp"

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
			request.fail(HTTP_CE_CONTENT_UNPROCESSABLE, "Declared max body size exceeded in current request (che scimmia che sei)");
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
