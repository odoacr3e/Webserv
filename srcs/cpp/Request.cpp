
#include "../hpp/Request.hpp"

/*
Host: localhost:8080
User-Agent: curl/8.17.1-DEV
Accept: /
Content-Length: 10 (SOLO POST)
Content-Type: application/x-www-form-urlencoded (SOLO POST)

	POST,
	GET,
	DELETE,
	HEAD,
*/
Request::Request()
{
	this->_validmethods[POST] = "POST";
	this->_validmethods[GET] = "GET";
	this->_validmethods[DELETE] = "DELETE";
	this->_validmethods[HEAD] = "HEAD";
	this->_max_method_length = 0;
	for (int i = 0; i != METH_NUM; i++)
		if (this->_validmethods[i].length() > this->_max_method_length)
			this->_max_method_length = this->_validmethods[i].length();
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
	this->_header.clear();
	this->_header["Connection"] = "keep-alive";
	this->_header["Transfer-Encoding"] = "unchunked";
	this->_status_code = HTTP_OK;
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
	std::cout << "\033[31mWARNING: METHOD " COLOR_RESET << this->_method;
	std::cout << "\033[31m HAS NO PARSING!!" COLOR_RESET << std::endl;
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

e_methods	Request::getMethodEnum() const
{
	int	i;

	for (int i = 0; i != METH_NUM; i ++)
	{
		if (this->_validmethods[i] == this->_method)
			return ((e_methods)(POST + i));
	}
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

size_t	Request::getBodyLen(void) const
{
	return (this->_body_len);
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

void	Request::setMethod(std::string method)
{
	this->_method = UNDEFINED;
	if (method.length() > this->_max_method_length)
		return ;
	for (int i = 0; i < this->getMethNum(); i++)
	{
		if (method == this->_validmethods[i])
		{
			this->_method = method;
			return ;
		}
	}
}

void	Request::setUrl(std::string url)
{
	if (url.find('\r') != std::string::npos)
	{
		this->_url = "";
		return ;
	}
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

void	Request::setBodyLen(size_t len)
{
	this->_body_len = len;
}

void	Request::setHeaderVal(std::string key, std::string val)
{
	std::cout << "\033[33mNEW:" COLOR_RESET << key << " " << val << "\n";
	//if (!checkKey(key))
	//	DBG_MSG("Key: " + key + " does not exist and has been added to header map");
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
	std::cout << "KEY PRE ERROR: " << key << std::endl;
	if (checkKey(key) == false)
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
