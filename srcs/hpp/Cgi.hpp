#ifndef CGI_HPP
# define CGI_HPP

# include "Server.hpp"
# include "Client.hpp"

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
	void	reset();
	void	clear();
	void	clear(Server &srv, Client &client);
	void	removeFromPoll(bool is_pipe_out, Server &srv);
	int		headerParsing(Client &client);
	int		readChunk(Client &client);

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

#endif

