
#include "../hpp/Client.hpp"

Client::Client(int sockfd)
{
	this->_sockfd = sockfd;
}

Client::~Client()
{}

Client::Client(const Client &other)
{
	*this = other;
}

Client	Client::operator=(const Client &other)
{
	if (this == &other)
		return (*this);
	this->_sockfd = other._sockfd;
	return (*this);
}
