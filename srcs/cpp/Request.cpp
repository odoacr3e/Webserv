
#include "../hpp/Request.hpp"

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
// 	true OK
//	false ERROR
int	Request::checkHeader(void)
{
	if (!this->checkVal("Host") || !this->checkVal("Accept") || !this->checkVal("User-Agent"))
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Host"));
	else if (!this->checkVal("Accept"))
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Accept"));
	else if (!this->checkVal("User-Agent"))
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing User-Agent"));
	if (this->_header["Transfer-Encoding"] != "chunked" && \
	this->_header["Transfer-Encoding"] != "unchunked")
		return (this->fail(HTTP_CE_BAD_REQUEST, "Invalid Transfer-Encoding"));
	if (this->_method == "POST")
		return (this->_checkPost());
	// else if (this->_method == "GET")
	// 	return (this->_checkGet());
	else if (this->_method == "DELETE")
		return (this->_checkDelete());
	std::cout << "\033[31mWARNING: METHOD " COLOR_RESET << this->_method;
	std::cout << "\033[31m HAS NO PARSING!!" COLOR_RESET << std::endl;
	return (0);
}

int	Request::_checkPost(void)
{
	if (!this->checkVal("Content-Type"))
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Content-Type"));
	if (this->_header["Transfer-Encoding"] == "unchunked")
		return (0);
	if (!this->checkVal("Content-Length"))
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Content-Length"));
	return (0);
}

int	Request::_checkGet(void)
{
	return (0);
}

int	Request::_checkDelete(void)
{
	if (!this->checkVal("Authorization"))
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Authorization"));
	return (0);
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
	ip = ipport.substr(0, ipport.find(':'));
	port = std::atoi(ipport.substr(ipport.find(':') + 1).c_str());
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

bool	Request::setHeaderVal(std::string key, std::string val)
{
	// std::cout << "\033[33mNEW:" COLOR_RESET << key << " " << val << "\n";
	if (key == "Content-Length" && !charFinder(val, std::isdigit))
		return (1);
	this->_header[key] = val;
	return (0);
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
	// std::cout << "\033[33mChecking key:\t" << "\033[0m" << key << "\n";
	if (checkKey(key) == false)
		return (false);
	// std::cout << key << "\033[32m è stata riempita!\n\033[0m";
	return (true);
}

int	Request::fail(e_http_codes code)
{
	return (this->fail(code, ""));
}

int	Request::fail(e_http_codes code, std::string info)
{
	int	int_code;

	int_code = code;
	switch (int_code)
	{
		case HTTP_CE_BAD_REQUEST :
			SWITCH_LOG(info, "ClientError: Bad Request");
		case HTTP_CE_UNATHORIZED :
			SWITCH_LOG(info, "ClientError: Unathorized");
		case HTTP_CE_FORBIDDEN:
			SWITCH_LOG(info, "ClientError: Forbidden");
		case HTTP_CE_NOT_FOUND:
			SWITCH_LOG(info, "ClientError: Not found");
		case HTTP_CE_METHOD_NOT_ALLOWED:
			SWITCH_LOG(info, "ClientError: Method not allowed");
		case HTTP_CE_CONFLICT:
			SWITCH_LOG(info, "ClientError: Another process is currently using the requested resource");
		case HTTP_CE_LENGTH_REQUIRED:
			SWITCH_LOG(info, "ClientError: Lenght required");
		case HTTP_CE_URI_TOO_LONG:
			SWITCH_LOG(info, "ClientError: Uri too long");
		case HTTP_CE_MEDIA_TYPE:
			SWITCH_LOG(info, "ClientError: Media type of requested file is unknown");
		case HTTP_CE_IM_TEAPOT:
			SWITCH_LOG(info, "ClientError: Want a cup of tea?" CHARIZARD);
		case HTTP_SE_NOT_IMPLEMENTED:
			SWITCH_LOG(info, "ServerError: Not implemented -->");
		default :
			SWITCH_LOG(info, "ClientError: Wtf" PIEDI_DELLA_ZIA_DEL_TUO_RAGAZZO);	
	}
	this->setStatusCode(code);
	this->setRequestErrorBool(true);
	return (1);
}

void	Request::manageIndex(t_conf_server *srv, t_conf_location *loc)
{
	if (!loc)
	{
		this->setUrl(this->getUrl().append(srv->index));
	}
	else
		this->setUrl(this->getUrl().append(loc->index));
}

// std::map<std::string, t_conf_location>
void	Request::findRightPath(t_conf_server *srv)
{
	typedef std::map<std::string, t_conf_location> maplocation;
	// controllo se esiste uri nelle location, altrimenti root server
	std::cout << "\033[33m FINDRIGHTPATH" COLOR_RESET;
	std::cout << "url: " << getUrl() << std::endl;
	std::string tmpuri;

	for (maplocation::iterator it = srv->location.begin(); it != srv->location.end(); ++it)
	{
		if ((*it).first.compare(this->getUrl().substr(0, (*it).first.length())) == 0 && \
			(*it).first.length() > tmpuri.length())
			tmpuri = (*it).first;
	}
	if (tmpuri.empty()) // se non trova location corretta
	{
		if (this->getUrl() == "/")//da spostare sopra!
			manageIndex(srv, NULL);
		this->setUrl(srv->root + this->getUrl());
	}
	else
	{
		/*
			URL =		/dir/
			tmpuri =	/dir/
			alias = 	/www/var/

			index.html
			/www/var/index.html
			&list[0 + uri[0] == '/'] che sesso che mi fai quando fai così
		*/
		if (!srv->location[tmpuri].alias.empty())
		{
			this->setUrl(this->getUrl().erase(0, tmpuri.length()));
			if (this->getUrl().empty())
				;//
			this->setUrl(srv->location[tmpuri].alias.append(this->getUrl()));
		}
		else if (!srv->location[tmpuri].root.empty())
			this->setUrl(srv->location[tmpuri].root + this->getUrl()); //root
		else
			this->setUrl(srv->root + this->getUrl());
	}
	std::cout << "RESULT " << this->getUrl() << " " << std::endl;
	//casi di uri
	/*
		location /
		location /cartella
		1) uri = "/"
			pseudocodice
		2) uri = "/index.html"
			pseudocodice
		3) uri = "/cartella/index.html"
			pseudocodice
		4) uri = "/cartella1/cartella2/index.html"
			pseudocodice
	*/
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
