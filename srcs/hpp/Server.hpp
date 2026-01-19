
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
	/*
		blocco server 1
		{
		listen 127.0.0.1:9001;# fd 0
		listen 127.0.0.2:9002;# fd 1
		listen 127.0.0.3:9003;# fd 2
		listen 127.0.0.4:9004;# fd 3
		}

		blocco server 2
		{
		listen 127.0.0.5:9005;# fd 4
		listen 127.0.0.6:9006;# fd 5
		listen 127.0.0.7:9007;# fd 6
		listen 127.0.0.8:9008;# fd 7
		}

		blocco server 1 --> fd: 0,1,2,3
		blocco server 2 --> fd: 4,5,6,7

		&blocco server 1
		&blocco server 2

		connessione fd 2
		fd 2 ---> &blocco server 1
	*/
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
		void			checkForConnection();
		int				getServerNum() const;
		SrvNameMap		&getSrvNameMap() const;

		void			printServerConfiguration(Conf &conf, SrvNameMap::iterator it) const;
};

void	convertDnsToIp(IpPortPair &ipport, SrvNameMap &srvmap);

#endif