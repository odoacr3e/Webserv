#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../../includes/ether.hpp"

class	Request;
class	Response;

class Response
{
	private:
	//canonic
	public:
		Response();
		~Response();
		Response(const Response &other);
		Response	operator=(const Response &other);

};

#endif