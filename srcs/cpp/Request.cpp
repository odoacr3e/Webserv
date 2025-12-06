
#include "../hpp/Request.hpp"

const std::string Request::_validmethods[METH_NUM] = {
    "POST",
    "GET",
    "DELETE"
};

/*
Host: localhost:8080
User-Agent: curl/8.17.1-DEV
Accept: /
Content-Length: 10 (SOLO POST)
Content-Type: application/x-www-form-urlencoded (SOLO POST)


*/
Request::Request()
{
	this->_method = "GET";
	reset_request();
}

/* void	Request::fill_checks(void)
{
	for (int i = 0; i < METH_NUM; i++)
		this->_validmethods[i] = VALID_METHODS[i];
} */

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
	this->_body = other._body;
	return (*this);
}

void	Request::reset_request(void)
{
	_header["Host"] = "";
	_header["User-Agent"] = "";
	_header["Accept"] = "";
	_header["Content-Length"] = "";
	_header["Content-type"] = "";
	_header["Connection"] = "";
}

std::string	Request::getValidMethod(int idx) const
{
	return (this->_validmethods[idx]);
}

std::string	Request::getMethod() const
{
	return (this->_method);
}

int Request::getMethNum() const
{
	return (METH_NUM);
}

std::string Request::getUrl() const
{
	return (this->_url);
}

std::string Request::getHttpVersion() const
{
	return (this->_http_version);
}

std::string Request::getBody() const
{
	return (this->_body);
}

std::map<std::string, std::string>	Request::getHeader() const
{
	return (this->_header);
}

std::string	Request::getHeaderVal(std::string key)
{
	if (this->_header.find(key) == this->_header.end())
		return ((std::string)"");
	else
		return (this->_header[key]);
}

void	Request::setMethod(int method)
{
	this->_method = this->_validmethods[method];
}

void	Request::setUrl(std::string url)
{
	this->_url = url;
}

void	Request::setHttpVersion(std::string version)
{
	this->_http_version = version;
}

void	Request::setBody(std::string body)
{
	this->_body = body;
}

void		Request::setHeaderVal(std::string key, std::string val)
{
	if (checkKey(key))
	{
		this->_header[key] = val;
		// std::cout << "header set at: " << key << std::endl; 
	}
	else
		std::cout << "Key: " + key + " does not exist in header map" << std::endl;
}

bool	Request::checkKey(std::string key)
{
	if (this->_header.find(key) != this->_header.end())
		return (false);
	return (true);
}

bool	Request::checkVal(std::string key)
{
	if (checkKey(key) == false)
		return (false);
	if (this->_header.find(key) != this->_header.end())
		return (false);
	return (true);
}

std::ostream &operator<<(std::ostream &os, Request &obj)
{
	os << "Method: " << obj.getMethod() << "\nURL: " << \
	obj.getUrl() << "\nVERSION: " << obj.getHttpVersion() << std::endl;
	return (os);
}