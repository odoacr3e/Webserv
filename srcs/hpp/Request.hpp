#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../../includes/ether.hpp"
# include "../../includes/status_codes.hpp"
# include "Conf.hpp"
# include <stdarg.h>

# define UNDEFINED ""
# define MAX_URI_LENGTH 8000
# define SWITCH_LOG(info, s) \
std::cerr << "\033[31mRequestError: \033[33m" << s << " \033[0m" << info << "\n"; break 

typedef struct s_conf_server	t_conf_server;
typedef std::map<std::string, std::string> headermap;
typedef std::pair<std::string, int>	IpPortPair;
typedef std::map<IpPortPair, t_conf_server> SrvNameMap;

enum	e_methods
{
	POST,
	GET,
	DELETE,
	HEAD,
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
		std::string		_validmethods[METH_NUM];
		headermap		_header;
		std::string		_method; // LINE
		std::string 	_url; // LINE
		std::string 	_http_version; // LINE
		std::string		_body;
		IpPortPair		_ipport;
		e_http_codes	_status_code;
		size_t			_max_method_length;
		size_t			_body_len;
		bool			_error; //GIORGIOVANNI

		int				_checkPost(void);
		int				_checkGet(void);
		int				_checkDelete(void);
	public:
		
		Request();
		~Request();
		Request(const Request &other);
		Request &operator=(const Request &other);

		//getters
		std::string		getValidMethod(int idx) const;
		int				getMethNum() const;
		std::string		getMethod() const;
		e_methods		getMethodEnum() const;
		std::string 	getUrl() const;
		std::string 	getHttpVersion() const;
		std::string 	getBody() const;
		headermap		&getHeader();
		std::string		getHeaderVal(std::string key);
		IpPortPair		&getHost();
		e_http_codes	getStatusCode() const;
		size_t			getBodyLen() const;
		bool			getRequestErrorBool() const;

		//setters
		void			setMethod(std::string method);
		void			setHttpVersion(std::string version);
		void 			setUrl(std::string);
		void			setBody(std::string);
		bool			setHeaderVal(std::string key, std::string val);
		void			setStatusCode(e_http_codes status_code);
		void			setRequestErrorBool(bool error);
		void			setBodyLen(size_t len);
		void			findRightPath(t_conf_server *srv);
		bool			checkVal(std::string key);
		bool			checkKey(std::string key);
		int				checkHeader(void);

		int				fail(e_http_codes code);
		int				fail(e_http_codes code, std::string info);
		void			resetRequest(void);
		void			printHeader(void);

};

std::ostream &operator<<(std::ostream &os, Request &obj);

#endif