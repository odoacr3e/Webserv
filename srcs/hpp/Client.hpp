#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../../includes/ether.hpp"
# include "Request.hpp"

class	Request;

class Client
{
	private:
		int 		_sockfd;
		int			_srvfd;
		Request		_request;

	public:
		Client(int sockfd, int srvfd);
		~Client();
		Client(const Client &other);
		Client	operator=(const Client &other);

		Request	&getRequest();

};

#endif