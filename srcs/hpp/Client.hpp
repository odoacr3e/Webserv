#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Request.hpp"
# include "Conf.hpp"
# include <vector>

typedef struct s_conf_server	t_conf_server;
typedef struct s_conf_location	t_conf_location;

struct s_conf_server;
struct s_conf_location;
struct s_cgi;
class	Request;

class Client
{
	private:
		bool				_send_content;
		int 				_sockfd;
		int					_srvfd;
		Request				_request;
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
		Request				&getRequest();
		t_conf_server		&getSrvConf();
		t_conf_location		&getLocConf();
		std::vector<char>	&getBuffer();
		struct pollfd		*getPollData();
		char				*getBufferChar();
		int					getAllowedMethods() const;
		int					isAllowedMethod();
		bool				&sendContentBool();
};

#endif