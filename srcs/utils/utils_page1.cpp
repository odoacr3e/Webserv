
#include "../../includes/ether.hpp"
#include "../../includes/status_codes.hpp"

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

int checkValidCode(int code)
{
	int	valid_codes[] = VALID_HTTP_CODES;
	int	size = sizeof(valid_codes) / sizeof(valid_codes[0]);

	for (int i = 0; i < size; i++)
	{
		if (code == valid_codes[i])
			return (i);
	}
	return (HTTP_UNKNOWN);
}

std::string	file_opener(std::fstream &file, std::string msg)
{
	std::string			ret;

	if (file.is_open())
	{
		std::getline(file, ret, '\0');
		file.close();
	}
	else
		std::cout << msg << "!\n";
	return (ret);
}

std::string	file_opener(std::fstream &file)
{
	return (file_opener(file, "cannot open file"));
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
// }"OK