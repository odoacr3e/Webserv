#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include "Request.hpp"
# include "Conf.hpp"
# include "Cgi.hpp"
# include <vector>

typedef struct s_conf_server	t_conf_server;
typedef struct s_conf_location	t_conf_location;
typedef struct s_cookieData		t_cookieData;
//
struct s_conf_server;
struct s_conf_location;
struct s_cgi;
struct s_cookieData;
class	Request;

class Client
{
	private:
		bool				_send_content;
		int 				_sockfd;
		int					_srvfd;
		int					_poll_index;
		Request				_request;
		t_cookieData		_cookie_data;
		t_conf_server		_srv_config;
		t_conf_location		_loc_config;
		std::vector<char>	_buffer;

	public:
		Client(int sockfd, int srvfd);
		~Client();
		Client(const Client &other);
		Client	operator=(const Client &other);

		//SECTION - getters
		int					getSockFd() const;
		int					&getPollIndex();
		Request				&getRequest();
		t_conf_server		&getSrvConf();
		t_conf_location		&getLocConf();
		std::string			getBodyType() const;
		std::vector<char>	&getBuffer();
		struct pollfd		*getPollFd(Server &srv);
		char				*getBufferChar();
		s_cookieData		&getCookieData();
		void				setCookieData(Server &srv);
		int					getAllowedMethods() const;
		int					isAllowedMethod();
		bool				&sendContentBool();
		void				bindCgiSocket(Server &srv, s_cgi &cgi);

		//SECTION - setters
		void				setBodyType(std::string type);
		//SECTION - cgi
		void				readCgi(Server &srv, s_cgi &cgi);
		void				writeCgi(Server &srv, s_cgi &cgi);
};

#endif