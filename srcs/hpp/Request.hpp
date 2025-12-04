#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../../includes/ether.hpp"

class	Request;
class	Response;

class Request
{
	private:
	//canonic
	public:
		Request();
		~Request();
		Request(const Request &other);
		Request	operator=(const Request &other);
};

#endif