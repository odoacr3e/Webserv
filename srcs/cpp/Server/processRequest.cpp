#include "../../hpp/Server.hpp"

void	Server::processRequest(Client &client, char *buffer, int bytes)
{
	Request	&request = client.getRequest();
	t_conf_server	srv;
	t_conf_location	loc;
	request.getRequestStream().str(buffer);
	request.getRequestStream().clear();
	request.getSockBytes() = bytes;

	print_file("REQUEST", buffer, bytes);
	if (request.getFirstRead() == true) // legge la prima volta
	{
		request.getFirstRead() = false;
		if (requestParsing(client, buffer, bytes) != 0)//request
		{
			client.getPollFd()->events = POLLOUT;
			// TODO - da settare status code corretto senza fare return ?????
			return ;
		}
		convertDnsToIp(request, request.getHost(), *this->_srvnamemap);// serverFinder
		if ((*this->_srvnamemap).count(request.getHost()) == 0)
		{
			client.getPollFd()->events = POLLOUT;
			request.setRequestErrorBool(true);
			return ;
		}
		if ((size_t)(*this->_srvnamemap)[request.getHost()].client_max_body_size < request.getBodyLen())
			request.fail(HTTP_CE_CONTENT_UNPROCESSABLE, "Declared max body size exceeded in current request (che scimmia che sei)");
		this->setupRequestEnvironment(client);
		std::cout << "PorcessRequest(): RUN_SCRIPT FLAG " << (client.getLocConf().run_script == false ? "false" : "true") << std::endl;
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
		std::cout << "processRequest(): " << client.getPollFd()->fd << ":POLLOUT" << std::endl;
		client.getPollFd()->events = POLLOUT;
		request.getFirstRead() = true;
	}
	std::cout << BLUE"processRequest(): \n" << request << RESET << std::endl;
	std::cout << "BODY\n" << request.getBinBody() << std::endl;
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

	srv = &(*this->_srvnamemap)[request.getHost()];
	client.getSrvConf() = *srv;
	loc = request.findRightLocation(srv);
	if (loc)
		client.getLocConf() = *loc;
	request.findRightUrl(&(*this->_srvnamemap)[request.getHost()], loc);
	if (client.isAllowedMethod() == 0)
		request.fail(HTTP_CE_METHOD_NOT_ALLOWED, "Ti puzzano i piedi (della zia del tuo ragazzo)");
}
