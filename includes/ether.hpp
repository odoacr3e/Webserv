#ifndef ETHER_HPP
# define ETHER_HPP
# include <iostream>
# include <string>
# include <poll.h>
# include <sys/unistd.h>
# include <errno.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <sys/poll.h>
# include <netinet/in.h>
# include <unistd.h>
# include <vector>
# include <map>
# include <cstring>
# include <cstdlib>
# include <sstream>

template <typename T>
std::string	ft_to_string(T data)
{
	std::ostringstream	stream;

	stream << data;
	return (stream.str());
}

#endif