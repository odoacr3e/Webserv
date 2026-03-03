
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

bool	&Client::sendContentBool()
{
	return (this->_send_content);
}

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

void	Client::writeCgi(Server &srv, s_cgi &cgi)
{
	write(cgi.pipe[1], cgi.input.c_str(), cgi.input.length());
	if (cgi.isFastCgiBool == false)
		cgi.removeFromPoll(true, srv);
	else
		srv.getAddrsVector()[cgi.pipe[0]].events &= (~POLLOUT);
}

s_cgi::s_cgi(void)
{
	this->isFastCgiBool = false;
	this->pid = 0;
	this->pipe[0] = 0;
	this->pipe[1] = 0;
	this->poll_index[0] = 0;
	this->poll_index[1] = 0;
	this->bytes_read = 0;
	this->output_len = 0;
	this->isParsed = false;
}

s_cgi::s_cgi(Client &client)
{
	this->bytes_read = 0;
	this->pipe[0] = 0;
	this->pipe[1] = 0;
	this->poll_index[0] = 0;
	this->poll_index[1] = 0;
	this->pid = 0;
	this->output_len = 0;
	this->isParsed = false;
	if (client.getLocConf().exist == true && client.getLocConf().fastcgi_bool == true)
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
	this->poll_index[0] = other.poll_index[0];
	this->poll_index[1] = other.poll_index[1];
	this->pid = other.pid;
	this->pipe[0] = other.pipe[0];
	this->pipe[1] = other.pipe[1];
	this->output = other.output;
	this->output_len = other.output_len;
	this->bytes_read = other.bytes_read;
	this->isFastCgiBool = other.isFastCgiBool;
	this->isParsed = other.isParsed;
	return (*this);
}

// return -1 if an error occurs, else 0
int		s_cgi::headerParsing(Client &client)
{
	int			bytes;
	std::string	output;

	client.getBuffer().resize(CGI_HEADER_LEN);
	bytes = read(this->pipe[0], client.getBuffer().data() + this->bytes_read, CGI_HEADER_LEN - this->bytes_read);
	if (bytes <= 0)
		return (std::cerr << "readCgiError: cgi is gone\n", -1);
	this->bytes_read += bytes;
	if (this->bytes_read != CGI_HEADER_LEN)
		return (0);
	client.getBuffer().push_back('\0');
	output = client.getBuffer().data();
	client.getBuffer().clear();
	this->bytes_read = 0;
	if (output.compare(0, 3, "OK|") != 0 || output.rbegin()[0] != '|')
		return (std::cerr << "readCgiErrorFormat: " << output << "\n", -1);
	bytes = std::atoi(output.c_str() + 3);
	if (bytes <= 0)
		return (std::cerr << "readCgiErrorSize: " << output << "\n", -1);
	this->output_len = bytes;
	this->isParsed = true;
	return (0);
}

int		s_cgi::readChunk(Client &client)
{
	int	bytes;

	client.getBuffer().resize(this->bytes_read + CHUNK_READ);
	bytes = read(this->pipe[0], client.getBuffer().data() + this->bytes_read, CHUNK_READ);
	if (bytes <= 0)
		return (-1);
	this->bytes_read += bytes;
	client.getBuffer().resize(this->bytes_read);
	//std::cout << "s_cgi::readChunk(): bytes read: " << this->bytes_read << "/";
	//std::cout << this->output_len << "\n";
	return (bytes);
}

void	s_cgi::removeFromPoll(bool is_pipe_out, Server &srv)
{
	int			fd_last;
	int			poll_index;

	poll_index = this->poll_index[is_pipe_out];
	if (!poll_index)
		return ;
	fd_last = srv.getAddrsVector().back().fd;
	if (srv.getFdData()[fd_last].cgi)
	{
		srv.getFdData()[fd_last].cgi->poll_index[0] = this->poll_index[0];
		srv.getFdData()[fd_last].cgi->poll_index[1] = this->poll_index[1];
	}
	if ((size_t)poll_index < srv.getAddrSize())
		std::swap(srv.getAddrsVector()[poll_index], srv.getAddrsVector().back());
	srv.getAddrsVector().pop_back();
	std::cout << "s_cgi::removeFromPoll() pipe fd: " << this->pipe[0] << "\n";
	close_fd(&this->pipe[is_pipe_out]);
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

void	s_cgi::clear()
{
	if (this->pid != 0 && this->isFastCgiBool == true)
		kill(this->pid, SIGKILL);
	this->pid = 0;
	close_fd(&this->pipe[0]);
	close_fd(&this->pipe[1]);
}

void	s_cgi::reset()
{
	this->bytes_read = 0;
	this->isParsed = false;
	this->input.clear();
	this->output = NULL;
	this->output_len = 0;
}
