#ifndef SERVER_HPP
# define SERVER_HPP

# include "../../includes/ether.hpp"
# include "Client.hpp"
# include "Request.hpp"

//	1024 perché lo fa anche nginx
# define MAX_CONNECTION 1024

//	se nel config non ce lo specificano
# define DEFAULT_PORT 8080

class Client;

class Server //classe Server(HTTP) -> gestisce piu ip:porta in contemporanea
{
	private:
		std::vector<struct pollfd>	_addrs; //pollfd per poll(), una struct per ogni ip:porta in ascolto
		std::map<int, Client *>		_clients;
	
	public:
		Server();
		~Server();

		void 			addSocket();
		struct pollfd	*getAddrs(void);
		size_t			getAddrSize(void) const;
		void			checkForConnection();
};

enum	e_http_codes
{
//NOTE - https://en.wikipedia.org/wiki/List_of_HTTP_status_codes

//SECTION - informational response: tells the client to wait for a final response
	HTTP_IR_CONTINUE = 100,//Expect: 100-continue is set in request
	HTTP_IR_SWITCH_PROT = 101,//server agrees on changing protocols.
	HTTP_IR_PROCESSING = 102,//(DEPRECATED) server is processing the request
	HTTP_IR_EARLY_HINTS = 103,//return some response headers.
//SECTION - success: tells the client to wait for a final response

};

#endif