
#ifndef SERVER_HPP
# define SERVER_HPP

# include "../../includes/ether.hpp"

//	1024 perché lo fa anche nginx
# ifndef MAX_CONNECTION
#  define MAX_CONNECTION 1024
# endif
# ifndef LISTENING_PORT
#  define LISTENING_PORT 8080
# endif

class Server //classe Server(HTTP) -> gestisce piu ip:porta in contemporanea
{
	private:
		std::vector<struct pollfd> _addrs; //pollfd per poll(), una struct per ogni ip:porta in ascolto

	public:
		Server();
		~Server();
		
		void 			addSocket();
		struct pollfd	*getAddrs(void);
		size_t			getAddrSize(void) const;
		void			checkForConnection();
};

#endif