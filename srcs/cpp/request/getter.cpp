#include "../../hpp/Request.hpp"

/*NOTE - index of content
		-	method
		-	url
		-	HttpVersion
		-	Headers
		-	Body
		-	Status code, error bool
		-	Flags (autoindex)
		-	SockData
*/

//SECTION - method

/*
{enum	e_methods}		{this->_method}	{array di metodi}
{
	POST,				"POST"			this->_validmethods[POST] = "POST";
	GET,				"GET"			this->_validmethods[GET] = "GET";
	DELETE,				"DELETE"		this->_validmethods[DELETE] = "DELETE";
	HEAD,				"HEAD"			this->_validmethods[HEAD] = "HEAD";
	METH_NUM,			NULL			ERROR		
};
*/

//idx dipende da enum e_methods
std::string	Request::getValidMethod(int idx) const
{
	return (this->_validmethods[idx]);
}

//torna il metodo della richiesta come stringa
std::string	Request::getMethod() const
{
	return (this->_method);
}

//numero di metodi consentiti
int Request::getMethNum() const
{
	return (METH_NUM);
}

//torna l'enum connessa a this->_method
e_methods	Request::getMethodEnum() const
{
	for (int i = 0; i != METH_NUM; i ++)
	{
		if (this->_validmethods[i] == this->_method)
			return ((e_methods)(POST + i));
	}
	return (METH_NUM);
}

//SECTION - url

//torna url come stringa
std::string Request::getUrl() const
{
	return (this->_url);
}

std::string Request::getUrlOriginal() const
{
	return (this->_url_orig);
}

//SECTION - HttpVersion

//torna sempre HTTP/1.1, altrimenti parsing fallisce
std::string Request::getHttpVersion() const
{
	return (this->_http_version);
}

//SECTION - Headers

//ritorna headermap reference
headermap	&Request::getHeader()
{
	return (this->_header);
}



//ritorna il valore della chiave, o stringa vuota se non esiste
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

//SECTION - body

//torna il body come stringa unica
std::string Request::getBody() const
{
	return (this->_body);
}

//torna la lunghezza del body.
//in fase di parsing request, puo essere diverso dalla body.length()
size_t	Request::getBodyLen(void) const
{
	return (this->_body_len);
}

std::string Request::getBodyType(void) const
{
	return (this->_body_type);
}

//SECTION - Status code, error bool

e_http_codes	Request::getStatusCode() const
{
	return (this->_status_code);
}

/*
	true se:
		-	StatusCode != 2xx
		-	DnsError
*/
bool	Request::getDnsErrorBool() const
{
	return (this->_error);
}

std::string	Request::getFailMsg() const
{
	return (this->_fail_msg);
}

//SECTION - Flags (autoindex)

bool	Request::getAutoIndexBool() const
{
	return (this->_autoindex);
}

bool	Request::getRunScriptBool() const
{
	return (this->_run_script);
}

//SECTION - SockData
char	*Request::getSockBuff()
{
	return (this->_sock_buff);
}

int		&Request::getSockBytes()
{
	return (this->_sock_bytes);
}

int		&Request::getSockFd()
{
	return (this->_sock_fd);
}

std::istringstream	&Request::getRequestStream()
{
	return (*this->_requestStream);
}

std::vector<char>	&Request::getBinBody()
{
	return (this->_bin_body);
}
