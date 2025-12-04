#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../../includes/ether.hpp"

class Request;
class Response;

#define VALID_METHODS "POST", "GET", "DELETE"

/*
##	CONTENUTO, DESCRIZIONE			ESEMPIO

METODO URI VERSIONE_HTTP		-	POST / HTTP/1.1
Host: porta client				-	Host: localhost:8080
User-Agent: ???					-	User-Agent: curl/8.17.1-DEV
ACCEPT: ???						-	Accept: \*\/\*
Content-Length: lunghezza msg	-	Content-Length: 10
	La lunghezza non deve
	superare un tot
	il tot va definito
Content-Type:	???				-	Content-Type: application/x-www-form-urlencoded
Connection: TYPE				-	Connection: keep-alive
Transfer-Encoding: ???			-	???
\n	(riga vuota)				-	\n
MESSAGGIO						-	ciao monco

_http:		TUTTO
_host:		localhost:8080
_content:	10
_uri:		/
*/

// FIXME - gestire transfer-encoding
class Request
{
private:
	enum e_methods
	{
		POST,
		GET,
		DELETE,
		METH_NUM,
	};
	int 		_method;
	std::string _url;
	std::string _http_version;
	std::string _host;
	int 		_contentlength;
	std::string _contenttype;
	std::string _connection;
	std::string _encoding;
	int 		_port;
	std::string _body;

	// void		fill_checks(void);

public:
	Request();
	~Request();
	Request(const Request &other);
	Request &operator=(const Request &other);

	//getters
	const int			getMethNum() const;
	const int			getMethod() const;
	const std::string 	getUrl() const;
	const std::string 	getHttpVersion() const;
	const std::string 	getHost() const;
	const int 			getContentLenght() const;
	const std::string 	getContentType() const;
	const std::string 	getConnection() const;
	const std::string 	getEncoding() const;
	const int 			getPort() const;
	const std::string 	getBody() const;
	
	//setters
	void 	setRequest(std::string); // format request
	int	 	setMethod(int method) const;
	void 	setUrl(std::string);
	void 	setHttpVersion(std::string);
	void 	setHost(std::string);
	void	setContentLenght(int);
	void	setContentType(std::string);
	void	setConnection(std::string);
	void 	setEncoding(std::string);
	void 	setPort(int);
	void	setBody(std::string);
};

#endif