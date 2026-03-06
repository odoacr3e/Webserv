
#include "../hpp/Client.hpp"

Client::Client(int sockfd, int srvfd)
{
	this->_sockfd = sockfd;
	this->_srvfd = srvfd;
	this->_send_content = false;
}

Client::~Client()
{}

Client::Client(const Client &other)
{
	*this = other;
}

Client	Client::operator=(const Client &other)
{
	if (this == &other)
		return (*this);
	this->_sockfd = other._sockfd;
	return (*this);
}

int	Client::getSockFd() const
{
	// std::cout << "fd restituito: " << this->_sockfd << std::endl;
	return (this->_sockfd);
}

Request	&Client::getRequest()
{
	return (this->_request);
}

t_conf_server	&Client::getSrvConf()
{
	return (this->_srv_config);
}

t_conf_location	&Client::getLocConf()
{
	return (this->_loc_config);
}

std::vector<char>	&Client::getBuffer()
{
	return (this->_buffer);
}

struct pollfd		*Client::getPollFd(Server &srv)
{
	return (&srv.getAddrs()[this->_poll_index]);
}

int					&Client::getPollIndex()
{
	return (this->_poll_index);
}

char				*Client::getBufferChar()
{
	return (this->_buffer.data());
}

/// @brief takes mask methods from location if exist, else from server
/// @return mask
int		Client::getAllowedMethods() const
{
	if (this->_loc_config.exist)
		return (this->_loc_config.mask_methods);
	else
		return (this->_srv_config.mask_methods);
}

s_cookieData		&Client::getCookieData()
{
	return (this->_cookie_data);
}

/// @brief set cookie data for client, if sent in request
/// @param srv 
void	Client::setCookieData(Server &srv)
{
	this->_cookie_data.exist = true;
	this->_cookie_data.id = this->getRequest().getCookieKey();
	this->_cookie_data.login = "client " + this->_cookie_data.id;
	this->_cookie_data.cgi = srv.getCookieMap()[this->_cookie_data.id].cgi;
}

//returns 0 if not allowed
//else returns method mask
int	Client::isAllowedMethod()
{
	e_methods	request_method;

	request_method = this->getRequest().getMethodEnum();
	return (this->getAllowedMethods() & (1 << request_method));
}

bool	&Client::sendContentBool()
{
	return (this->_send_content);
}

/// @brief	binds a cgi and a client sockets. In detail:
///			1)		updates the poll array, according to cgi type;
///			1.1)		cgi: adds the socket of the cgi;
///			1.2)		fastcgi: set socket to POLLIN;
///			2)		set client to POLLHUP (waiting to cgi);
///			3)		updates poll metadata (Server::_fd_data)
/// @param srv 
/// @param cgi 
void	Client::bindCgiSocket(Server &srv, s_cgi &cgi)
{
	if (cgi.isFastCgiBool == false)
		cgi.poll_index[0] = srv.addSocket(cgi.pipe[0], FD_PIPE_RD);
	else
		srv.getAddrs()[cgi.poll_index[0]].events = POLLIN;
	this->getPollFd(srv)->events = POLLHUP;
	srv.getFdData()[cgi.pipe[0]].client = this;
	srv.getFdData()[cgi.pipe[0]].cgi = &cgi;
	srv.getFdData()[this->getSockFd()].cgi_ready = true;
	srv.getFdData()[this->getSockFd()].client = this;
	srv.getFdData()[this->getSockFd()].cgi = &cgi;
}

/// @brief parse cgi header, reads cgi output
/// @param srv 
/// @param cgi reference to binded cgi
void	Client::readCgi(Server &srv, s_cgi &cgi)
{
	int	error;

	if (cgi.bytes_read == 0)
		this->getBuffer().clear();
	if (cgi.isParsed == false)
		error = cgi.headerParsing(*this);
	else
		error = cgi.readChunk(*this);
	if (error == -1)
	{
		std::cerr << "cgi error\n";
		if (cgi.pid)
			{kill(cgi.pid, SIGKILL); cgi.pid = 0;}
	}
	else if (cgi.bytes_read != cgi.output_len)
		return ;
	//LOG_CGI(this->getBuffer());
	cgi.output = this->getBufferChar();
	this->getPollFd(srv)->events = POLLOUT;
	if (cgi.isFastCgiBool == false)
	{
		cgi.clear(srv, *this);
		srv.getPollIndex()-= 1;
	}
	else
	{
		srv.getAddrsVector()[cgi.poll_index[0]].events &= (~POLLIN);
		srv.getAddrsVector()[cgi.poll_index[0]].revents &= (~POLLIN);
	}
}

/// @brief client writes to cgi pipe[1]
/// @param srv 
/// @param cgi reference to binded cgi
void	Client::writeCgi(Server &srv, s_cgi &cgi)
{
	write(cgi.pipe[1], cgi.input.c_str(), cgi.input.length());
	if (cgi.isFastCgiBool == false)
		cgi.removeFromPoll(true, srv);
	else
		srv.getAddrsVector()[cgi.pipe[0]].events &= (~POLLOUT);
}
