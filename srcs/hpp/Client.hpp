#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../../includes/ether.hpp"
# include "Request.hpp"
// # include "Response.hpp"

class	Request;
class	Response;

class Client
{
	private:
		int 		_sockfd;
		int			_srvfd;
		Request		_request;
		// Response	_response;

	public:
		Client(int sockfd, int srvfd);
		~Client();
		Client(const Client &other);
		Client	operator=(const Client &other);

		Request	&getRequest();

};

#endif