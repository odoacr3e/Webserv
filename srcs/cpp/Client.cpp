
#include "../hpp/Client.hpp"

Client::Client(int sockfd, int srvfd)
{
	this->_sockfd = sockfd;
	this->_srvfd = srvfd;
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

Request	&Client::getRequest()
{
	return (this->_request);
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


