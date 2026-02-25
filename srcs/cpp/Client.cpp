
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

struct pollfd		*Client::getPollFd()
{
	return (this->_poll_fd);
}

char				*Client::getBufferChar()
{
	return (this->_buffer.data());
}

int		Client::getAllowedMethods() const
{
	if (this->_loc_config.exist)
		return (this->_loc_config.mask_methods);
	else
		return (this->_srv_config.mask_methods);
}

//returns 0 if not allowed
//else returns method mask
int	Client::isAllowedMethod()
{
	e_methods	request_method;

	request_method = this->getRequest().getMethodEnum();
	return (this->getAllowedMethods() & (1 << request_method));
}

bool				&Client::sendContentBool()
{
	return (this->_send_content);
}

void				Client::setPollFd(struct pollfd *p)
{
	this->_poll_fd = p;
}

void	Client::readCgi(Server &srv, s_cgi &cgi)
{
	std::string filename("/dev/fd/" + ft_to_string(cgi.pipe[0]));
	std::cout << "readCgi():\nfilename: " << filename << "\n";
	if (cgi.isFastCgiBool == true)
	{
		if (read_fastcgi(*this, cgi) == 1)
			return (std::cerr << "fastCgi error\n", (void)0);//FIXME - uccidere fastcgi
	}
	else
		read_file(filename, this->getBuffer());
	print_file("CGI", "----\nREAD FROM CGI:\n----\n");
	print_file("CGI", this->getBuffer());
	print_file("CGI", "\n----\n");
	cgi.output = this->getBufferChar();
	this->getPollFd()->events = POLLOUT;
	std::cout << WHITE "readCgi(): client " RESET << this->getSockFd() << " POLLOUT\n";
	std::cout << WHITE "readCgi(): pipe[0] " RESET << cgi.pipe[0] << " rimosso da poll\n";
	if (cgi.isFastCgiBool == false)
		cgi.clear(srv, *this);
	else
	{
		srv.getAddrsVector()[cgi.poll_index[0]].events &= (~POLLIN);
		srv.getAddrsVector()[cgi.poll_index[0]].revents &= (~POLLIN);
	}
}

void	Client::writeCgi(Server &srv, s_cgi &cgi)
{
	std::string filename("/dev/fd/" + ft_to_string(cgi.pipe[1]));
	std::cout << "writeCgi():\nfilename: " << filename << "\n";
	write(cgi.pipe[1], cgi.input.c_str(), cgi.input.length());
	print_file("CGI", "----\nWRITE TO CGI:\n----\n");
	print_file("CGI", cgi.input);
	print_file("CGI", "\n----\n");
	if (cgi.isFastCgiBool == false)
		cgi.removeFromPoll(true, srv);
	else
		srv.getAddrsVector()[cgi.pipe[0]].events &= (~POLLOUT);
}

s_cgi::s_cgi(void)
{
	this->client = NULL;
	this->isFastCgiBool = false;
	this->pid = 0;
	this->pipe[0] = 0;
	this->pipe[1] = 0;
	this->poll_index[0] = 0;
	this->poll_index[1] = 0;
}

s_cgi::s_cgi(Client &client)
{
	this->client = &client;
	this->pipe[0] = 0;
	this->pipe[1] = 0;
	this->poll_index[0] = 0;
	this->poll_index[1] = 0;
	this->pid = 0;
	if (client.getLocConf().exist == true && client.getLocConf().script_daemon == true)
		this->isFastCgiBool = true;
	else
		this->isFastCgiBool = false;
}

s_cgi::s_cgi(const s_cgi &other)
{
	*this = other;
}

s_cgi	&s_cgi::operator=(const s_cgi &other)
{
	if (this == &other)
		return (*this);
	this->client = other.client; 
	this->poll_index[0] = other.poll_index[0];
	this->poll_index[1] = other.poll_index[1];
	this->pid = other.pid;
	this->pipe[0] = other.pipe[0];
	this->pipe[1] = other.pipe[1];
	this->output = other.output;
	this->isFastCgiBool = other.isFastCgiBool;
	return (*this);
}

void	s_cgi::removeFromPoll(bool is_pipe_out, Server &srv)
{
	int			fd_last;
	int			poll_index;

	poll_index = this->poll_index[is_pipe_out];
	fd_last = srv.getAddrsVector().back().fd;
	if (srv.getFdData()[fd_last].cgi)
	{
		srv.getFdData()[fd_last].cgi->poll_index[0] = this->poll_index[0];
		srv.getFdData()[fd_last].cgi->poll_index[1] = this->poll_index[1];
	}
	if ((size_t)poll_index != srv.getAddrSize() - 1)
		std::swap(srv.getAddrsVector()[poll_index], srv.getAddrsVector().back());
	srv.getAddrsVector().pop_back();
	std::cout << WHITE"Prima di entrare pipe[0]: " RED << this->pipe[0] << "" RESET << std::endl;
	std::cout << WHITE"Flag is_pipe_out: " RED << (is_pipe_out == false ? "false" : "true") << "" RESET << std::endl;
	if (is_pipe_out == false)
	{
		std::cout << WHITE"Entro e chiudo pipe[0]: " RED << this->pipe[0] << "" RESET << std::endl;
		close_fd(&this->pipe[0]);
	}
	else
	{
		std::cout << WHITE"Entro e chiudo pipe[1]: " RED << this->pipe[1] << "" RESET << std::endl;
		close_fd(&this->pipe[1]);
	}
}

void	s_cgi::clear(Server &srv, Client &client)
{
	IpPortPair	&ipPort = client.getRequest().getHost();

	if (this->pid != 0 && this->isFastCgiBool == true)
		kill(this->pid, SIGKILL);
	this->pid = 0;
	if (this->pipe[0])
		this->removeFromPoll(false, srv);
	if (this->pipe[1])
		this->removeFromPoll(true, srv);
	if (srv.getIpPortCgiMap().find(ipPort) != srv.getIpPortCgiMap().end())
		srv.getIpPortCgiMap().erase(ipPort);
}
