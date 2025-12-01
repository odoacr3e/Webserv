
#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <poll.h>
# include <unistd.h>
# include <errno.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <sys/poll.h>
# include <netinet/in.h>
# include <unistd.h>
# include <vector>
# include <cstring>
# include <cstdlib>
# include <exception>

class Server
{
	private:
		std::vector<struct pollfd> _addrs;

	public:
		Server();
		~Server();
		
		void 			addSocket();
		struct pollfd	*getAddrs(void);
		size_t			getAddrSize(void) const;
		void			checkForConnection();
};


#endif