#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../../includes/ether.hpp"
# include "../../includes/status_codes.hpp"

# define UNDEFINED ""//method undefined
# define MAX_URI_LENGTH 8000//see status code HTTP_CE_URI_TOO_LONG
# define SWITCH_LOG(info, s) \
std::cerr << "\033[31mRequestError: \033[33m" << s << " \033[0m" << info << "\n"; \
info = s;break 

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

enum	e_methods_mask
{
	MASK_NO_METHODS = 0,
	MASK_POST = 1 << POST,
	MASK_GET = 1 << GET,
	MASK_DELETE = 1 << DELETE,
	MASK_HEAD = 1 << HEAD,
	MASK_ALL_METHODS = (1 << 8) - 1,
};

// FIXME - gestire transfer-encoding

class Request
{
	private:
		std::string			_validmethods[METH_NUM];//array di metodi validi
		headermap			_header;//std::map<key, value> relativi a headers
		std::istringstream 	_requestStream;
		std::string			_method;
		std::string 		_url;
		std::string 		_url_orig;
		std::string 		_http_version;//sempre uguale a HTTP/1.1
		std::string			_body;
		std::string			_fail_msg;
		std::vector<char>	_bin_body;
		std::string			_body_type;
		std::string			_cookie_key;
		IpPortPair			_ipport;
		e_http_codes		_status_code;
		size_t				_max_method_length;
		size_t				_body_len;
		char				*_sock_buff;
		int					_sock_bytes;
		int					_sock_fd;
		int					_bytes_first_recvd;
		int					_bytes_left;
		bool				_error;
		bool				_autoindex;
		bool				_run_script;
		bool				_first_read;
		bool				_body_headers_bool;
		int					_checkPost(void);
		int					_checkGet(void);
		int					_checkDelete(void);

	public:
	//ANCHOR - Request.cpp
	//SECTION - canonic form
		Request();
		~Request();
		Request(const Request &other);
		Request &operator=(const Request &other);
	//SECTION - checks
		void			findRightUrl(t_conf_server *srv, t_conf_location *loc);
		t_conf_location	*findRightLocation(t_conf_server *srv);
		bool			checkVal(std::string key);
		int				fail(e_http_codes code);
		bool			checkKey(std::string key);
		int				checkHeader(void);
	//SECTION - fail
		int				fail(e_http_codes code, std::string info);
		int				fail(int code, std::string info);
	//SECTION - request uri modifiers
		void			manageIndex(t_conf_server *srv, t_conf_location *loc);
		void			resetRequest(void);

	//ANCHOR - getters.cpp
		std::string					getValidMethod(int idx) const;
		std::string 				getUrl() const;
		std::string					getMethod() const;
		std::string 				getHttpVersion() const;
		std::string					getHeaderVal(std::string key);
		std::string 				getBody() const;
		std::string 				getUrlOriginal() const;
		std::string					getFailMsg() const;
		int							getMethNum() const;
		e_methods					getMethodEnum() const;
		headermap					&getHeader();
		IpPortPair					&getHost();
		std::string					&getCookieKey();
		size_t						getBodyLen() const;
		std::string 				getBodyType() const;
		e_http_codes				getStatusCode() const;
		bool						getDnsErrorBool() const;
		bool						getAutoIndexBool() const;
		bool						getRunScriptBool() const;
		bool						&getFirstRead();
		bool						&getBodyHeaders();
		char						*getSockBuff();
		int							&getSockBytes();
		int							&getSockFd();
		int							&getBytesLeft();
		std::istringstream			&getRequestStream();
		std::vector<char>			&getBinBody();

		//ANCHOR - setters.cpp
		void			setMethod(std::string method);
		void 			setUrl(std::string);
		void 			setUrlOriginal(std::string);
		void			setHttpVersion(std::string version);
		bool			setHeaderVal(std::string key, std::string val);
		void			setBody(std::string);
		void			setBodyLen(size_t len);
		void 			setBodyType(std::string type);
		void			setStatusCode(e_http_codes status_code);
		void			setRequestErrorBool(bool error);
		void			setParsingData(int fd, int bytes, char *buf);

		//ANCHOR - print.cpp
		void			printHeader(void);
};

std::ostream &operator<<(std::ostream &os, Request &obj);

#endif