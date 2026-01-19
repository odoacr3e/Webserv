
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
	resetRequest();
}

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
	this->_header["Connection"] = "keep-alive";
	this->_header["Transfer-Encoding"] = "unchunked";
	this->_header["Authorization"] = "";
	this->_error = false;
}

// FIXME Quetsa funzione controlla che se uno dei membri richiesti è assente 
// restituisce l'errore di corrispondenza e questa cosa va gestita, per ora restituiamo
// true e false
int	Request::checkHeader(void)
{
	if (!this->checkVal("Host") || !this->checkVal("Accept") || !this->checkVal("User-Agent"))
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
	if (!this->checkVal("Content-Length") || !this->checkVal("Content-Type"))
			return (false);
	return (true);
}

int	Request::_checkGet(void)
{
	return (true);
}

int	Request::_checkDelete(void)
{
	if (!this->checkVal("Authorization"))
		return (false);
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

headermap	&Request::getHeader()
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

//setta la pair se gia non e stata settata
IpPortPair	&Request::getHost()
{
	std::string	ipport;
	std::string	ip;
	int			port;

	if (!this->_ipport.first.empty())
		return (this->_ipport);
	ipport = this->_header["Host"];
	// std::cout << "excuseme wtf?: " << this->_header["Host"] << "\n";
	ip = ipport.substr(0, ipport.find(':'));
	port = std::atoi(ipport.substr(ipport.find(':') + 1).c_str());
	// std::cout << "convert: " << ipport << " ip: " << ip << " port: " << port << "\n";
	this->_ipport = IpPortPair(ip, port);	
	return (this->_ipport);
}

e_http_codes	Request::getStatusCode() const
{
	return (this->_status_code);
}

bool	Request::getRequestErrorBool() const
{
	return (this->_error);
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

void	Request::setHeaderVal(std::string key, std::string val, SrvNameMap &srv_names)
{
	(void)srv_names;
	if (val.find(' ') != std::string::npos)
	{
		DBG_MSG("Val: " + val + " stores one or more spaces");
		return ;
	}
	if (!checkKey(key))
		DBG_MSG("Key: " + key + " does not exist and has been added to header map");
	this->_header[key] = val;
}

void	Request::setStatusCode(e_http_codes status_code)
{
	this->_status_code = status_code;
}

void	Request::setRequestErrorBool(bool error)
{		
	this->_error = error; 
}

bool	Request::checkKey(std::string key)
{
	if (this->_header.find(key) == this->_header.end())
		return (false);
	return (true);
}

bool	Request::checkVal(std::string key)
{
	std::cout << "\033[33mChecking key:\t" << "\033[0m" << key << "\n";
	if (checkKey(key) == false)
		return (false);
	if (this->_header.find(key) == this->_header.end())
		return (false);
	std::cout << key << "\033[32m è stata riempita!\n\033[0m";
	return (true);
}

void	Request::printHeader(void)
{
	for (headermap::const_iterator it = _header.begin(); it != _header.end(); it++)
	{
		std::cout << "Key: " << it->first << std::endl;
		std::cout << "Value: " << it->second << std::endl;
	}
}

std::ostream &operator<<(std::ostream &os, Request &obj)
{
	os << "Method: " << obj.getMethod() << "\nURL: " << \
	obj.getUrl() << "\nVERSION: " << obj.getHttpVersion() << std::endl;
	std::cout << "\033[32m--Request header print--\n" COLOR_RESET;
	obj.printHeader();
	std::cout << "\033[32mStatusCode: " COLOR_RESET << obj.getStatusCode();
	std::cout << std::endl;
	return (os);
}
