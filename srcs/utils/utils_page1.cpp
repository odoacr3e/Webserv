
#include "../../includes/ether.hpp"

std::string	removeWhitespaces(std::string line)
{
	size_t 		i = 0;

	if (line[i] != 0 && !std::isspace(line[0]))
		return (line);
	while (line[i] != 0 && std::isspace(line[i]))
		i++;	
	return (line.substr(i));
}

size_t	find_first_not_whitespace(std::string line)
{
	size_t i = 0;

	while(line[i] != 0 && std::isspace(line[i]))
		i++;
	return(i);
}

size_t	find_first_whitespace(std::string line)
{
	size_t i = 0;

	while(line[i] != 0 && !std::isspace(line[i]))
		i++;
	return(i);
}

bool	is_there_whitespaces(std::string line)
{
	size_t i = 0;

	while(line[i] != 0 && !std::isspace(line[i]))
	{
		return(true);
		i++;
	}
	return(false);
}


//
//http    xvzv   dsdcsdf 
// {
//  	server
// 	{
// 		location
// 		{
		
// 		}
// 		location
// 		{
		
// 		}
// 	}
// 	server
// 	{
		
// 	}
// }
