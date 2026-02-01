#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../../includes/ether.hpp"
# include "../../includes/status_codes.hpp"
# include "Conf.hpp"

# define UNDEFINED ""//method undefined
# define MAX_URI_LENGTH 8000//see status code HTTP_CE_URI_TOO_LONG
# define SWITCH_LOG(info, s) \
std::cerr << "\033[31mRequestError: \033[33m" << s << " \033[0m" << info << "\n"; break 

typedef struct s_conf_server	t_conf_server;
typedef struct s_conf_location	t_conf_location;

typedef std::map<std::string, std::string> headermap;
typedef std::pair<std::string, int>	IpPortPair;
typedef std::map<IpPortPair, t_conf_server> SrvNameMap;

//SECTION - enum e_methods
/*
	per aggiungere un nuovo metodo:
	1)	inserirlo qui;
	2)	in Request.cpp:
		2.1)	aggiungerlo al costruttore di Request (Request.cpp);
		2.2)	fare funzione di Request chiamata check[nome nuovo metodo];
		2.3)	richiamare la nuova funzione in checkHeader;
	3)	in parseRequest.cpp amplia lo switch in bodyParsing;
*/
enum	e_methods
{
	POST,
	GET,
	DELETE,
	HEAD,
	METH_NUM,
};
// FIXME - gestire transfer-encoding

class Request
{

	private:
		std::string		_validmethods[METH_NUM];//array di metodi validi
		headermap		_header;//std::map<key, value> relativi a headers
		std::string		_method;
		std::string 	_url;
		std::string 	_url_orig;
		std::string 	_http_version;//sempre uguale a HTTP/1.1
		std::string		_body;
		std::string		_body_type;
		IpPortPair		_ipport;
		e_http_codes	_status_code;
		size_t			_max_method_length;
		size_t			_body_len;
		bool			_error;
		bool			_autoindex;
		bool			_run_script;

		int				_checkPost(void);
		int				_checkGet(void);
		int				_checkDelete(void);

	public:
	//ANCHOR - Request.cpp
	//SECTION - canonic form
		Request();
		~Request();
		Request(const Request &other);
		Request &operator=(const Request &other);
	//SECTION - checks
		void			findRightUrl(t_conf_server *srv);
		t_conf_location	*findRightLocation(t_conf_server *srv);
		bool			checkVal(std::string key);
		bool			checkKey(std::string key);
		int				checkHeader(void);
	//SECTION - fail
		int				fail(e_http_codes code);
		int				fail(e_http_codes code, std::string info);
	//SECTION - request uri modifiers
		void			manageIndex(t_conf_server *srv, t_conf_location *loc);
		void			resetRequest(void);

	//ANCHOR - getters.cpp
		std::string		getValidMethod(int idx) const;
		std::string		getMethod() const;
		int				getMethNum() const;
		e_methods		getMethodEnum() const;
		std::string 	getUrl() const;
		std::string 	getUrlOriginal() const;
		std::string 	getHttpVersion() const;
		headermap		&getHeader();
		std::string		getHeaderVal(std::string key);
		IpPortPair		&getHost();
		std::string 	getBody() const;
		size_t			getBodyLen() const;
		std::string 	getBodyType() const;
		e_http_codes	getStatusCode() const;
		bool			getDnsErrorBool() const;
		bool			getAutoIndexBool() const;
		bool			getRunScriptBool() const;
	//ANCHOR - setters.cpp
		void			setMethod(std::string method);
		void 			setUrl(std::string);
		void			setHttpVersion(std::string version);
		bool			setHeaderVal(std::string key, std::string val);
		void			setBody(std::string);
		void			setBodyLen(size_t len);
		void 			setBodyType(std::string type);
		void			setStatusCode(e_http_codes status_code);
		void			setRequestErrorBool(bool error);
	//ANCHOR - print.cpp
		void			printHeader(void);
};

std::ostream &operator<<(std::ostream &os, Request &obj);

#endif