#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Request.hpp"
# include "Server.hpp"
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
		struct pollfd		*_poll_fd;
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
		struct pollfd		*getPollFd();
		char				*getBufferChar();
		int					getAllowedMethods() const;
		int					isAllowedMethod();
		bool				&sendContentBool();

		//SECTION - setters

		void				setPollFd(struct pollfd *p);

		//SECTION - cgi
		void				readToCgi(Server &srv, s_cgi &cgi);
		void				writeToCgi(Server &srv, s_cgi &cgi);
};

typedef	struct s_cgi
{
	s_cgi(void);
	s_cgi(Client &client);
	s_cgi(const s_cgi &other);
	s_cgi	&operator=(const s_cgi &other);
	void	clear(Server &srv, Client &client);
	void	removeFromPoll(bool is_pipe_out, Server &srv);

	Client		*client;
	std::string	input;
	char		*output;
	int			argv_len[2];
	int			pipe[2];
	int			poll_index[2];
	int			pid;
	bool		isFastCgiBool;
}		t_cgi;

#endif