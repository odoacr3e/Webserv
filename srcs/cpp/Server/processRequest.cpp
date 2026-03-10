#include "../../hpp/Server.hpp"

/**
 * @brief Takes in the request and processes it:
 * 
 * Parses it using requestParsing(), converts the DNS using convertDnsToIp() in case servername isnt an ip
 * 
 * - receives the first part of the request in recvFirstReqPart() GET and DELETE
 * 
 * - receives the rest of the request, in case its a POST (because of the binary file information) in recvRemainingReqParts()
 * 
 * - then checks How many bites were ridden and what status code we have in  checkBytesAndScode()
 * @param client > Client containing the request
 * @param buffer > Buffer used on recv, contains all the data sent by the request
 * @param bytes > Number of byutes read
 */
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

/**
 * @brief Handles the first part of the request, usually containing the header and body
 *
 * - For \p `POST` it handles the first part of the request, containing the header and part of the body
 *
 * - For \p `GET` it handles the whole request, header and body
 * 
 * @param client > Client containing the request
 * @param buffer > Buffer used on recv, contains all the data sent by the request
 * @param bytes > Number of byutes read
 * @return \p `true` if ok, \p `false` if an error occured 
 */
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
	if (request.getUrl().find("🍵") != std::string::npos)
		return (request.fail(HTTP_CE_IM_TEAPOT));
	if (client.getRequest().getMethodEnum() == GET)
		request.getBytesLeft() -= request.getBodyLen();
	return (true);
}

/**
 * @brief In case of POST, fills the std::vector<Char> we use for storing binary files
 *
 * @param client > Client containing the request 
 */
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

/**
 * @brief this function checks how many bytes were ridden and updates the class variables
 *
 * then checks the requets's status code
 * 
 * @param client > Client containing the request
 */
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

/**
 * @brief Converts the dominion name in a numeric ip address
 * 
 * @param request > Request containing the dominion
 * @param ipport > Pair ip - port
 * @param srvmap > Program's SrvNameMap
 */
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
				LOG_TERM << " ----> " << ipport.first << "\n";
				return ;
			}
		}
	}
	request.setRequestErrorBool(true);
}
/**
 * @brief checks location params
 * 
 * @param client > Client containing request
 */
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
	if (request.getUrl().rbegin()[0] == '?')
		request.setUrl(request.getUrl().erase(request.getUrl().length() - 1, 1));
	request.findRightUrl(&(*this->_srvnamemap)[request.getHost()], loc);
	if (client.isAllowedMethod() == 0)
		request.fail(HTTP_CE_METHOD_NOT_ALLOWED, "Methods allowed: POST, GET, DELETE");
	if (client.getRequest().getCookieKey().empty() == false)
		client.setCookieData(*this);
}
