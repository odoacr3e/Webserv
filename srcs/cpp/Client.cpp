
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
	std::cout << "fd restituito: " << this->_sockfd << std::endl;
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

	// void 	setRequest(std::string); // format request
	// void	setMethNum(int);
	// void 	setMethod(int);
	// void 	setUrl(std::string);
	// void 	setHttpVersion(std::string);
	// void 	setHost(std::string);
	// void	setContentLenght(int);
	// void	setContentType(std::string);
	// void	setConnection(std::string);
	// void 	setEncoding(std::string);
	// void 	setPort(int);
	// void	setBody(std::string);

	// int 		_method;
	// std::string _url;
	// std::string _http_version;
	// std::string _host;
	// int 		_contentlength;
	// std::string _contenttype;
	// std::string _connection;
	// std::string _encoding;
	// int 		_port;
	// std::string _body;


