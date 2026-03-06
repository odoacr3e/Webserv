#include "../../hpp/Server.hpp"

// void	Server::processRequest(Client &client, char *buffer, int bytes)
// {
// 	Request	&request = client.getRequest();
// 	request.getRequestStream().str(buffer);
// 	request.getRequestStream().clear();
// 	request.getSockBytes() = bytes;

// 	LOG_REQUEST(buffer, bytes);
// 	if (request.getFirstRead() == true) // legge la prima volta
// 	{
// 		request.getFirstRead() = false;
// 		if (requestParsing(client, buffer, bytes) != 0)//request
// 		{
// 			client.getPollFd(*this)->events = POLLOUT;
// 			return ;
// 		}
// 		convertDnsToIp(request, request.getHost(), *this->_srvnamemap);// serverFinder
// 		if ((*this->_srvnamemap).count(request.getHost()) == 0)
// 		{
// 			client.getPollFd(*this)->events = POLLOUT;
// 			request.setRequestErrorBool(true);
// 			return ;
// 		}
// 		this->setupRequestEnvironment(client);
// 		if (client.getRequest().getMethodEnum() == GET)
// 			request.getBytesLeft() -= request.getBodyLen();
// 	}
// 	else if (bodyHeaderParsing(request) == true)
// 	{
// 		request.getBinBody().insert(request.getBinBody().end(), request.getSockBuff(), request.getSockBuff() + request.getSockBytes());
// 		request.getBytesLeft() -= request.getSockBytes();
// 	}
// 	if (request.getBytesLeft() == 0)
// 	{
// 		client.getPollFd(*this)->events = POLLOUT;
// 		request.getFirstRead() = true;
// 	}
// 	else if (request.getBytesLeft() < 0 || request.getStatusCode() == HTTP_CE_METHOD_NOT_ALLOWED)
// 		client.getPollFd(*this)->events = POLLOUT;
// }

void	Server::processRequest(Client &client, char *buffer, int bytes)
{
	Request	&request = client.getRequest();

	request.getRequestStream().str(buffer);
	request.getRequestStream().clear();
	request.getSockBytes() = bytes;
	LOG_REQUEST(buffer, bytes);
	if (request.getFirstRead() == true)
	{
		if(recvFirstReqPart(client, buffer, bytes) == false)
			return;
	}
	else 
		recvRemainingReqParts(client);
	checkBytesAndScode(client);
}

bool	Server::recvFirstReqPart(Client &client, char *buffer, int bytes)
{
	Request &request = client.getRequest();

	request.getFirstRead() = false;
	if (requestParsing(client, buffer, bytes) != 0)//request
	{
		client.getPollFd(*this)->events = POLLOUT;
		return (false);
	}
	convertDnsToIp(request, request.getHost(), *this->_srvnamemap);
	if ((*this->_srvnamemap).count(request.getHost()) == 0)
	{
		client.getPollFd(*this)->events = POLLOUT;
		request.setRequestErrorBool(true);
		return (false);
	}
	this->setupRequestEnvironment(client);
	if (client.getRequest().getMethodEnum() == GET)
		request.getBytesLeft() -= request.getBodyLen();
	return (true);
}

void	Server::recvRemainingReqParts(Client &client)
{
	Request 			&request = client.getRequest();
	std::vector<char>	&bin_body = request.getBinBody();
	int					&read_bytes = request.getSockBytes();
	int					&bytes_left = request.getBytesLeft();

	if (bodyHeaderParsing(request) == true)
	{
		bin_body.insert(bin_body.end(), request.getSockBuff(), request.getSockBuff() + read_bytes);
		bytes_left -= read_bytes;
	}
}

void	Server::checkBytesAndScode(Client &client)
{
	Request			&request = client.getRequest();
	int				&bytes_left = request.getBytesLeft();
	e_http_codes	status_code = request.getStatusCode();

	if (bytes_left == 0)
	{
		client.getPollFd(*this)->events = POLLOUT;
		request.getFirstRead() = true;
	}
	else if (bytes_left < 0 || status_code == HTTP_CE_METHOD_NOT_ALLOWED)
		client.getPollFd(*this)->events = POLLOUT;
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
	int				max_body_size;

	srv = &(*this->_srvnamemap)[request.getHost()];
	max_body_size = srv->client_max_body_size;
	client.getSrvConf() = *srv;
	loc = request.findRightLocation(srv);
	if (loc)
	{
		client.getLocConf() = *loc;
		max_body_size = loc->client_max_body_size;
	}
	if (max_body_size < (int)client.getRequest().getBodyLen())
		request.fail(HTTP_CE_CONTENT_UNPROCESSABLE, "Declared max body size exceeded in current request");
	request.findRightUrl(&(*this->_srvnamemap)[request.getHost()], loc);
	if (client.isAllowedMethod() == 0)
		request.fail(HTTP_CE_METHOD_NOT_ALLOWED, "Methods allowed: POST, GET, DELETE");
	if (client.getRequest().getCookieKey().empty() == false)
		client.setCookieData(*this);
}
