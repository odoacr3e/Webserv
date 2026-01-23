
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
// restituisce l'errore di corrispondenza e questa cosa va gestita, per ora restituiamo
int	Request::checkHeader(void)
{
	if (this->checkVal("Host") || this->checkVal("Accept") || this->checkVal("User-Agent"))
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Host"));
	else if (this->checkVal("Accept"))
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Accept"));
	else if (this->checkVal("User-Agent"))
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
	std::cout << "\033[31m HAS NO PARSING!!" COLOR_RESET << std::endl << std::endl;
	return (0);
}

int	Request::_checkPost(void)
{
	if (this->checkVal("Content-Type"))
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Content-Type"));
	if (this->_header["Transfer-Encoding"] == "unchunked")
		return (0);
	if (this->checkVal("Content-Length"))
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Content-Length"));
	return (0);
}

int	Request::_checkGet(void)
{
	return (0);
}

int	Request::_checkDelete(void)
{
	if (this->checkVal("Authorization"))
		return (this->fail(HTTP_CE_BAD_REQUEST, "Missing Authorization"));
	return (0);
}

bool	Request::checkKey(std::string key)
{
	if (this->_header.find(key) == this->_header.end())
		return (1);
	return (0);
}

bool	Request::checkVal(std::string key)
{
	// std::cout << "\033[33mChecking key:\t" << "\033[0m" << key << "\n";
	if (checkKey(key) == 1)
		return (1);
	return (this->_header[key].empty());
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
	this->setStatusCode(code);
	this->setRequestErrorBool(true);
	return (1);
}

//SECTION - request uri modifiers

// std::map<std::string, t_conf_location>
void	Request::findRightPath(t_conf_server *srv)
{
	typedef std::map<std::string, t_conf_location> maplocation;
	// controllo se esiste uri nelle location, altrimenti root server
	std::cout << "\033[33m FINDRIGHTPATH: " COLOR_RESET;
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
			this->setUrl(srv->location[tmpuri].alias + this->getUrl());
		}
		else if (!srv->location[tmpuri].root.empty())
			this->setUrl(srv->location[tmpuri].root + this->getUrl()); //root
		else
			this->setUrl(srv->root + this->getUrl());
	}
	std::cout << "\t---> RESULT: " << this->getUrl() << " " << std::endl << std::endl;
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

void	Request::manageIndex(t_conf_server *srv, t_conf_location *loc)
{
	// no location: prendi index / autoindex del server
	if (!loc)
	{
		if (srv->autoindex == true)
			;//autoindex function
		else
			this->setUrl(this->getUrl().append(srv->index));
		return ;
	}
	if (loc->index.empty() == false)
		this->setUrl(this->getUrl().append(loc->index));
	else if (loc->autoindex == true)
		;//autoindex function
	else
		;//FORBIDDEN: autoindex e location non settate
}
