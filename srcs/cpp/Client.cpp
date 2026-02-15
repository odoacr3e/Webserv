
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


