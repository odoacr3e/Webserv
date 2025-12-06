
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
	resetRequest();
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

void	Request::resetRequest(void)
{
	this->_header["Host"] = "";
	this->_header["User-Agent"] = "";
	this->_header["Accept"] = "";
	this->_header["Content-Length"] = "";
	this->_header["Content-Type"] = "";
	this->_header["Connection"] = "";
	this->_header["Transfer-Encoding"] = "unchunked";
}

// FIXME Quetsa funzione controlla che se uno dei membri richiesti è assente 
// restituisce l'errore di corrispondeznza e questa cosa va gestita, per ora resituiamo
// true e false
int	Request::checkHeader(void)
{
	if (!this->checkVal("Host") || \
		!this->checkVal("Accept") || \
		!this->checkVal("User-Agent"))
		return (false);
	if (this->_method == "POST")
		return (this->_checkPost());
	else if (this->_method == "GET")
		return (this->_checkGet());
	else if (this->_method == "DELETE")
		return (this->_checkDelete());
	return (false);
}

int	Request::_checkPost(void)
{
	if (!this->checkVal("Content-Length") || \
		!this->checkVal("Content-Type"))
			return (false);
	return (true);
}

int	Request::_checkGet(void)
{
	return (true);
}

int	Request::_checkDelete(void)
{
	return (true);
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

std::map<std::string, std::string>	&Request::getHeader()
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
	}
	else
		std::cout << "Key: " + key + " does not exist in header map" << std::endl;
}

bool	Request::checkKey(std::string key)
{
	if (this->_header.find(key) == this->_header.end())
		return (false);
	return (true);
}

bool	Request::checkVal(std::string key)
{
	std::cout << "\033[33mChecking key:\t" + key + "\033[0m\n";
	if (checkKey(key) == false)
		return (false);
	if (this->_header.find(key) == this->_header.end())
		return (false);
	std::cout << key + "\033[32m è stata riempita!\n\033[0m";
	return (true);
}

void	Request::printHeader(void)
{
	for (std::map<std::string, std::string>::const_iterator it = _header.begin(); it != _header.end(); it++)
	{
		std::cout << "Key: " << it->first << std::endl;
		std::cout << "Value: " << it->second << std::endl;
	}
}

std::ostream &operator<<(std::ostream &os, Request &obj)
{
	os << "Method: " << obj.getMethod() << "\nURL: " << \
	obj.getUrl() << "\nVERSION: " << obj.getHttpVersion() << std::endl;
	return (os);
}
