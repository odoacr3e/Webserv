
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
		if (std::isspace(line[i]))
			return (true);
		i++;
	}
	return(false);
}


size_t	find_first_not_special_char(std::string line)
{
	size_t i = 0;

	while(line[i] != 0 && (std::isspace(line[i]) || std::strchr("{};", line[i])))
		i++;
	return(i);
}

size_t	find_first_special_char(std::string line)
{
	size_t i = 0;

	while(line[i] != 0 && !std::isspace(line[i]) && !std::strchr("{};#", line[i]))
		i++;
	return(i + (line[0] == '{' || line[0] == '}' || line[0] == ';' || line[0] == '#'));
}

bool	is_there_special_char(std::string line)
{
	size_t i = 0;

	while(line[i] != 0)
	{
		if (std::isspace(line[i]) || !std::strchr("{};", line[i]))
			return(true);
		i++;
	}
	return(false);
}

//REVIEW - Verifica che la funzione sia verificata in tutta la stringa
bool charFinder(const std::string literal, int(*check)(int))
{
	int index;
	int end;

	index = 0;
	end = literal.size();
	
	while (!literal.empty() && index < end)
	{
		if (check(literal[index]) == false)
			return (false);
		index++;
	}
	return (true);
}

//
//     /{  :; http    xvzv   dsdcsdf 
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