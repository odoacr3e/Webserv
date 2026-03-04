#ifndef CGI_HPP
# define CGI_HPP

# include "Server.hpp"
# include "Client.hpp"

typedef std::vector<char *>	argvVector;

class Server;
class Client;

// HEADER: 14 bytes ----> OK|0123456789|
//	"OK":		status of the cgi
//	0123456789: the bytes to read	
#define CGI_HEADER_LEN 14

typedef	struct s_cgi 
{
	s_cgi(void);
	s_cgi(Client &client);
	s_cgi(const s_cgi &other);
	s_cgi	&operator=(const s_cgi &other);
	void	exec(Server &srv, Client &client);
	void	reset();
	void	clear(bool isChildProcess);
	void	clear(Server &srv, Client &client);
	void	removeFromPoll(bool is_pipe_out, Server &srv);
	int		headerParsing(Client &client);
	int		readChunk(Client &client);
	void	processOutput(Client &client, std::string &body);

	std::string	input;
	char		*output;
	int			output_len;
	int			bytes_read;
	int			argv_len[2];
	int			pipe[2];
	int			poll_index[2];
	int			pid;
	bool		isFastCgiBool;
	bool		isParsed;
}		t_cgi;

void		get_argv(Client &client, argvVector &v);
void		exec_cgi(Server &srv, Client &client, t_cgi &cgi_data, argvVector &argv);
void		exec_fastcgi(Server &srv, Client &client, t_cgi &cgi_data, argvVector &argv);

std::string		createHtmlPokedex(s_cgi &cgi);
std::string		createHtmlCub(t_cgi &cgi_data, Client &client);
std::string		createHtmlYouTube(t_cgi &cgi_data);
std::string		createHtmlCrypter(t_cgi &cgi_ptr);
std::string		createHtmlWeaksleep(t_cgi &cgi_data);

#endif

