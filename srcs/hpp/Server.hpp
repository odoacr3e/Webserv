
#ifndef SERVER_HPP
# define SERVER_HPP

# include "../../includes/ether.hpp"
# include "Client.hpp"
# include "Conf.hpp"
# include "Request.hpp"

#define MSG_END_CONNECTION "======================================\nchiudo connessione {INDEX}\n======================================\n"

//	1024 perché lo fa anche nginx
# define MAX_CONNECTION 1024

//	se nel config non ce lo specificano
# define DEFAULT_PORT 8080

# define DEFAULT_ROUTE "0.0.0.0"

//Inizializza una struct pollfd settando l'fd a -1.
//utilizzato per tornare errore.
# define CONNECTION_FAIL (struct pollfd){-1, -1, -1}

enum e_fd_type
{
	FD_SERVER,
	FD_CLIENT,
	FD_PIPE_RD,
	FD_PIPE_WR,
};

class Client;
typedef std::vector<char *>	packetBuffer;
typedef std::map<IpPortPair, s_cgi>	ipPortCgiMap;
struct s_fd_data;
typedef std::vector<struct s_fd_data> fdData;

class Server //classe Server(HTTP) -> gestisce piu ip:porta in contemporanea
{
	private:
		std::vector<struct pollfd>		_addrs; //pollfd per poll(), una struct per ogni ip:porta in ascolto
		std::vector<struct s_fd_data>	_fd_data;
		std::map<int, Client *>			_clients;
		std::map<int, t_conf_server *>	_server_data;
		SrvNameMap						*_srvnamemap;
		ipPortCgiMap					_ipPortCgiPair;
		packetBuffer					_packet_buffer;
		std::string						_request_buffer;
		std::string						_protected_files;
		const char						**_env;
		int								_server_num;

	public:
		Server(Conf &conf, const char **env);
		~Server();

		void 				addSocket(int index);
		struct pollfd		*getAddrs(void);
		fdData				&getFdData(void);
		size_t				getAddrSize(void) const;
		void				processRequest(std::vector<struct pollfd>::iterator &it, char *buffer, int bytes);
		void				processResponse(std::vector<pollfd>::iterator &it);
		void				checkForConnection();
		void				eraseClient(std::vector<pollfd>::iterator &it);
		int					getServerNum() const;
		SrvNameMap			&getSrvNameMap() const;
		ipPortCgiMap		&getIpPortCgiMap();
		packetBuffer		&getPacketBuffer();
		const std::string	&getProtectedFiles() const;

		void				listDirectoriesAutoIndex(Client &client, std::string &body, std::string &url, dirent *cont);
		void				printServerConfiguration(SrvNameMap::iterator it) const;
		void				choose_file(Client &client, std::fstream &file, std::string url);
		void				createAutoindex(Client &client, std::string &body);
		void				setupRequestEnvironment(Client &client);
		std::string			createResponse(Client &client);
		std::string			checkErrorPages(Request &request);

		// NOTE - Methods functions
		void				runMethod(Client &client, std::string &body, std::fstream &file);
		void				getMethod(Client &client, std::string &body, std::fstream *file);
		void				deleteMethod(Client &client, std::string &body, std::fstream *file);
		void				postMethod(Client &client, std::string &body, std::fstream *resp_file);

		// NOTE - close all sockets
		void				suppressSocket();
};

typedef	struct s_cgi
{
//	s_cgi	&operator=(s_cgi &other){return (other);};
	std::string	output;
	Client		*client;
	char		*argv[3];
	int			argv_len[2];
	int			pipe[2];
	int			pid;
	int			client_fd;
}		t_cgi;

typedef struct s_fd_data
{
	enum e_fd_type	type;
}		t_fd_data;

void		ft_to_string(std::vector<char *> &packets, std::string &request_buff);
void		convertDnsToIp(Request &request, IpPortPair &ipport, SrvNameMap &srvmap);

#endif