
#include "../../hpp/Request.hpp"

/*NOTE - index of content
		-	Request class
		-	checks
		-	fail (status code print + set)
		-	request uri modifiers
*/
//SECTION - request class

Request::Request()
{
	this->_validmethods[POST] = "POST";
	this->_validmethods[GET] = "GET";
	this->_validmethods[DELETE] = "DELETE";
	this->_validmethods[HEAD] = "HEAD";
	this->_max_method_length = 0;
	this->_first_read = true;
	this->_bytes_left = 0;
	this->_body_headers_bool = false;
	for (int i = 0; i != METH_NUM; i++)
		if (this->_validmethods[i].length() > this->_max_method_length)
			this->_max_method_length = this->_validmethods[i].length();
	resetRequest();
}

void	Request::resetRequest(void)
{
	this->_header.clear();
	this->_header["Connection"] = "keep-alive";
	this->_header["Transfer-Encoding"] = "unchunked";
	this->_status_code = HTTP_OK;
	this->_error = false;
	this->_autoindex = false;
	this->_run_script = false;
	this->_body_len = 0;
}

Request::~Request()
{}

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

Request::Request(const Request &other)
{
	*this = other;
}


//SECTION - checks

// FIXME Quetsa funzione controlla che se uno dei membri richiesti è assente 
// restituisce l'errore di corrispondenza
int	Request::checkHeader(void)
{
	if (this->checkVal("Host") == false)
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Host"));
	else if (this->checkVal("Accept") == false)
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Accept"));
	else if (this->checkVal("User-Agent") == false)
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing User-Agent"));
	if (this->_header["Transfer-Encoding"] != "chunked" && \
	this->_header["Transfer-Encoding"] != "unchunked")
		return (this->fail(HTTP_CE_BAD_REQUEST, "Invalid Transfer-Encoding"));
	if (this->_method == "POST")
		return (this->_checkPost());
	else if (this->_method == "DELETE")
		return (this->_checkDelete());
	std::cout << "METHOD ==> |"<< this->_method << "|\n";
	// std::cout << "\033[31m HAS NO PARSING!!" COLOR_RESET << std::endl << std::endl;
	return (0);
}

int	Request::_checkPost(void)
{
	if (this->checkVal("Content-Type") == false)
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Content-Type"));
	if (this->_header["Transfer-Encoding"] == "unchunked")
		return (0);
	if (this->checkVal("Content-Length") == false)
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Content-Length"));
	return (0);
}

int	Request::_checkGet(void)
{
	return (0);
}

int	Request::_checkDelete(void)
{
	//if (this->checkVal("Authorization") == false)
	//	return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Authorization"));
	return (0);
}

// NOTE - torna true se la chiave esiste, altrimenti false
bool	Request::checkKey(std::string key)
{
	if (this->_header.find(key) == this->_header.end())
		return (false);
	return (true);
}

/*
	se non c'è la chiave:	FALSE
	se non c'è il valore:	FALSE
	se c'è e il valore:		TRUE
*/
bool	Request::checkVal(std::string key)
{
	// std::cout << "\033[33mChecking key:\t" << "\033[0m" << key << "\n";
	if (checkKey(key) == false)
		return (false);
	return (this->_header[key].empty() == false ? true : false);
	// std::cout << key << "\033[32m è stata riempita!\n\033[0m";
}

//SECTION - fail (status code print + set)

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
		case HTTP_CE_CONTENT_UNPROCESSABLE:
			SWITCH_LOG(info, "ClientError: request cannot be processed");
		case HTTP_SE_NOT_IMPLEMENTED:
			SWITCH_LOG(info, "ServerError: Not implemented -->");
		default :
			SWITCH_LOG(info, "ClientError: Wtf" PIEDI_DELLA_ZIA_DEL_TUO_RAGAZZO);	
	}
	this->_fail_msg = info;
	if (this->_fail_msg.empty() == true)
		this->_fail_msg = "Err";
	this->setStatusCode(code);
	return (1);
}

t_conf_location	*Request::findRightLocation(t_conf_server *srv)
{
	typedef std::map<std::string, t_conf_location> maplocation;
	std::string	url_longest;
	std::string	url_temp;
	std::string	url_request;

	url_request = this->_url;
	trim_from(url_request, url_request.find_last_of('/') + 1);//+1 skip /
	url_normalize(&url_request);
	for (maplocation::iterator it = srv->location.begin(); it != srv->location.end(); ++it)
	{
		url_temp = url_normalize((*it).first);
		if (url_temp == (url_request.substr(0, url_temp.length())) && \
		url_temp.length() > url_longest.length())
			url_longest = url_temp;
	}
	if (url_longest.empty())
		return (NULL);
	return (&(*srv).location[url_longest]);
}

// std::map<std::string, t_conf_location>
void	Request::findRightUrl(t_conf_server *srv)
{
	// controllo se esiste uri nelle location, altrimenti root server
	// std::cout << "\033[33m FINDRIGHTPATH: " COLOR_RESET;
	// std::cout << "url: " << getUrl() << std::endl;
	t_conf_location *loc;

	this->_url_orig = this->_url;
	loc = this->findRightLocation(srv);
	if (this->getUrl().rbegin()[0] == '/')
		manageIndex(srv, loc);
	else if (loc != NULL && loc->run_script == true)
		this->_run_script = true;
	if (loc)
		this->_url = url_rooting(this->_url, *loc);
	else
		this->_url = url_rooting(this->_url, *srv);
	// std::cout << "\t---> RESULT: " << this->getUrl() << "\n";
	std::cout << "findRightUrl(): " << this->getUrl() << std::endl;
}

// NOTE - controlliamo se autoindex e index sono settati e li impostiamo
void	Request::manageIndex(t_conf_server *srv, t_conf_location *loc)
{
	if (!loc)
		return (this->setUrl(this->getUrl().append(srv->index)));
	if (loc->autoindex == true)
	{
		this->_autoindex = true;
	}
	else
		this->setUrl(this->getUrl().append(loc->index));
}
