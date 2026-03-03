#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Request.hpp"
# include "Server.hpp"
# include "Conf.hpp"
# include "Cgi.hpp"
# include <vector>

typedef struct s_conf_server	t_conf_server;
typedef struct s_conf_location	t_conf_location;
//
struct s_conf_server;
struct s_conf_location;
struct s_cgi;
struct s_login;
class	Request;

class Client
{
	private:
		bool				_send_content;
		int 				_sockfd;
		int					_srvfd;
		int					_poll_index;
		Request				_request;
		// s_login				*_login;
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
		std::vector<char>	&getBuffer();
		struct pollfd		*getPollFd(Server &srv);
		char				*getBufferChar();
		int					getAllowedMethods() const;
		int					isAllowedMethod();
		bool				&sendContentBool();
		void				bindCgiSocket(Server &srv, s_cgi &cgi);

		//SECTION - cgi
		void				readCgi(Server &srv, s_cgi &cgi);
		void				writeCgi(Server &srv, s_cgi &cgi);
};

#endif