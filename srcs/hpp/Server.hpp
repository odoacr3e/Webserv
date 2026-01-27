
#ifndef SERVER_HPP
# define SERVER_HPP

# include "../../includes/ether.hpp"
# include "Client.hpp"
# include "Conf.hpp"
# include "Request.hpp"

//	1024 perché lo fa anche nginx
# define MAX_CONNECTION 1024

//	se nel config non ce lo specificano
# define DEFAULT_PORT 8080

# define DEFAULT_ROUTE "0.0.0.0"

//Inizializza una struct pollfd settando l'fd a -1.
//utilizzato per tornare errore.
# define CONNECTION_FAIL (struct pollfd){-1, -1, -1}

class Client;

class Server //classe Server(HTTP) -> gestisce piu ip:porta in contemporanea
{
	private:
		std::vector<struct pollfd>		_addrs; //pollfd per poll(), una struct per ogni ip:porta in ascolto
		std::map<int, Client *>			_clients;
		std::map<int, t_conf_server *>	_server_data;
		SrvNameMap						*_srvnamemap;
		int								_server_num;

	public:
		Server(Conf &conf);
		~Server();

		void 			addSocket(int index);
		struct pollfd	*getAddrs(void);
		size_t			getAddrSize(void) const;
		void			processRequest(std::vector<struct pollfd>::iterator it, char *buffer);
		void			checkForConnection();
		int				getServerNum() const;
		SrvNameMap		&getSrvNameMap() const;

		void			printServerConfiguration(Conf &conf, SrvNameMap::iterator it) const;
		void			choose_file(Client &client, std::fstream &file, std::string url);
		void			createAutoindex(Client &client, std::string &body);
		std::string		createResponse(Client &client);
		std::string		checkErrorPages(Request &request);
		void			runMethod(Client &client, std::string &body, std::fstream &file);

};

void	convertDnsToIp(Request &request, IpPortPair &ipport, SrvNameMap &srvmap);

#endif