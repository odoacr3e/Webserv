
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
struct s_cgi;
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

		int					addSocket(int index, e_fd_type type);
		struct pollfd		*getAddrs(void);
		std::vector<pollfd>	&getAddrsVector(void);
		fdData				&getFdData(void);
		size_t				getAddrSize(void) const;
		void				processRequest(Client &client, char *buffer, int bytes);
		void				processResponse(Client &client);
		void				checkForConnection();
		void				eraseClient(Client &client, int i);
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
		void 				print_info(void);
};

/*SECTION - gestione concorrenza

	4 strutture:				
	1)ARRAY poll: array di pollfd. Contiene tutti gli fd che POSSONO bloccare:
(enum e_fd_type)
	.FD_SERVER,
	.FD_CLIENT,
	.FD_PIPE_RD,
	.FD_PIPE_WR
		##VARIABILI
			Server::std::vector<struct pollfd>		_addrs;
		##GETTERS
			struct pollfd		*Server::getAddrs(void);
			std::vector<pollfd>	&Server::getAddrsVector(void);
			size_t				Server::getAddrSize(void) const;
		##ESEMPIO
				[0]			[1]			[2]			[3]			[4]
				fd srv 1	fd srv 2	fd cli 1	fd cli 2	fd pipe[0]

	2)VECTOR fdData: array che descrive il tipo degli fd nell'array poll
		##VARIABILI
			Server::std::vector<struct s_fd_data>	_fd_data;
		##GETTERS
			fdData				&Server::getFdData(void);
		##ESEMPIO
				[fd srv 1]	[fd srv 2]	[fd cli 1]	[fd cli 2]	[fd pipe[0]]
enum e_fd_type:		SERVER	SERVER		CLIENT		CLIENT		FD_PIPE_RD
Client *pointer:	NULL	NULL		*self		*self <-----*
s_cgi	*pointer:	NULL	NULL		NULL		*-------->	*self
bool	cgi_ready:	false	false		false		false/true	false

	3)	CLASS client (vedi Client.hpp)
	4)	struct s_cgi
		##VARIABILI
			Client * //FIXME - togliere
			std::string	input (input dato alla cgi)
			char 		*output (output della cgi, viene client::_buffer)
			char 		*argv[3]
			int			pipe[2]
			int			poll_index[2] --> posizione pipes nell'array di poll
			int			pid
			bool		isFastCgiBool
		##METODI
			s_cgi::clear 			(chiude tutto)
			s_cgi::removeFromPoll	(rimuove pipe lettura/scrittura da ar. poll)
*/
typedef struct s_fd_data
{
	s_cgi			*cgi;
	Client			*client;
	enum e_fd_type	type;
	bool			cgi_ready;
}		t_fd_data;

void		ft_to_string(std::vector<char *> &packets, std::string &request_buff);
void		convertDnsToIp(Request &request, IpPortPair &ipport, SrvNameMap &srvmap);

#endif