
#include "../hpp/Request.hpp"

Request::Request()
{
	//this->fill_checks();
}

/*
void	Request::fill_checks(void)
{
	for (int i = 0; i < METH_NUM; i++)
		this->_validmethods[i] = VALID_METHODS[i];
}*/

Request::~Request()
{

}

Request::Request(const Request &other)
{
	*this = other;
}

Request&	Request::operator=(const Request &other)
{
	if (this == &other)
		return (*this);
	this->_method = other._method;
	this->_url = other._url;
	this->_http_version = other._http_version;
	this->_host = other._host;
	this->_contentlength = other._contentlength;
	this->_contenttype = other._contenttype;
	this->_connection = other._connection;
	this->_encoding = other._encoding;
	this->_port = other._port;
	this->_body = other._body;
	return (*this);
}

const int	Request::getMethod() const
{
	return (this->_method);
}

const int Request::getMethNum() const
{
	return (this->METH_NUM);
}

const std::string Request::getUrl() const
{
	return (this->_url);
}

const std::string Request::getHttpVersion() const
{
	return (this->_http_version);
}

const std::string Request::getHost() const
{
	return (this->_host);
}

const std::string Request::getContentLenght() const
{
	return (this->_contentlength);
}

const std::string Request::getContentType() const
{
	return (this->_contenttype);
}

const std::string Request::getConnection() const
{
	return (this->_host);
}

const std::string Request::getEncoding() const
{
	return (this->_encoding);
}

const std::string Request::getPort() const
{
	return (this->_port);
}

const std::string Request::getBody() const
{
	return (this->_body);
}

void	Request::setMethod(int method)
{
	this->_method = method
}

void	Request::setUrl(std::string url)
{
	this->_url = url;
}

void	Request::setHttpVersion(std::string httpversion)
{
	this->_http_version = http_version;
}

void	Request::setHost(std::string host)
{
	this->_host = host;
}

void	Request::setContentLength(std::string contentlength)
{
	this->_contentlength = contentlength;
}

void	Request::setContentType(std::string contenttype)
{
	this->_contenttype = contenttype;
}

void	Request::setConnection(std::string connection)
{
	this->_connection = connection;
}

void	Request::setEncoding(std::string encoding)
{
	this->_encoding = encoding;
}

void	Request::setPort(std::string port)
{
	this->_port = port;
}

void	Request::setBody(std::string body)
{
	this->_body = body;
}