#ifndef ETHER_HPP
# define ETHER_HPP

# include <iostream>
# include <fstream>
# include <ostream>
# include <sstream>
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

# include "../srcs/hpp/Request.hpp"

# define DEBUG 1

#if defined(DEBUG) && DEBUG
  #define DBG_MSG(msg) (std::cout << "\033[31m" << (msg) << "\033[0m" << std::endl)
#else
  #define DBG_MSG(msg) ((void)0)
#endif

class Request;

template <typename T>
std::string	ft_to_string(T data)
{
	std::ostringstream	stream;

	stream << data;
	return (stream.str());
}

int			requestParsing(Request &request, std::string input);

//SEXTION - utils
std::string	removeWhitespaces(std::string line);
size_t		find_first_not_whitespace(std::string line);
size_t		find_first_whitespace(std::string line);
bool		is_there_whitespaces(std::string line);

size_t		find_first_special_char(std::string line);
bool		is_there_special_chars(std::string line);
size_t		find_first_not_special_char(std::string line);

#endif