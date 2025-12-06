#ifndef REQUEST_HPP
#define REQUEST_HPP

# include "../../includes/ether.hpp"
# include <stdarg.h>

# define UNDEFINED ""

enum	e_methods
{
	POST,
	GET,
	DELETE,
	METH_NUM,
};

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
		static const std::string			_validmethods[METH_NUM];
		std::map<std::string, std::string>	_header;
		std::string							_method; // LINE
		std::string 						_url; // LINE
		std::string 						_http_version; // LINE
		std::string							_body;
		bool								_checkPost(void);
		bool								_checkGet(void);
		bool								_checkDelete(void);

	public:
		Request();
		~Request();
		Request(const Request &other);
		Request &operator=(const Request &other);

		//getters
		std::string							getValidMethod(int idx) const;
		int									getMethNum() const;
		std::string							getMethod() const;
		std::string 						getUrl() const;
		std::string 						getHttpVersion() const;
		std::string 						getBody() const;
		std::map<std::string, std::string>	&getHeader();
		std::string							getHeaderVal(std::string key);

		//setters
		void								setMethod(int method);
		void								setHttpVersion(std::string version);
		void 								setUrl(std::string);
		void								setBody(std::string);
		void								setHeaderVal(std::string key, std::string val);
		bool								checkVal(std::string key);
		bool								checkKey(std::string key);
		bool								checkHeader(void);

		void								resetRequest(void);
		void								printHeader(void);

};

std::ostream &operator<<(std::ostream &os, Request &obj);

#endif