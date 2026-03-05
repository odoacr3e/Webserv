
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
/**
 * @brief check if a function is verified in all chars of a string
 * 
 * @param literal string to check
 * @param check function to check every char of literal
 * @return \p `true` if verified, \p `false` if not
 */
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

/**
 * @brief check if \p `code` passed is valid, checking into a macro set \p `VALID_HTTP_CODES`
 * 
 * @param code code to check
 * @return code checked if verified, \p `UNKNOWN` if not
 */
int checkValidHttpCode(int code)
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

std::string	open_and_read(std::fstream &file, std::string msg)
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

std::string	open_and_read(std::fstream &file)
{
	return (open_and_read(file, "cannot open file"));
}

/**
 * @brief Opens an fstream using <filename>
 * 
 * @param filename 
 * @return true => if open was successful 
 * 
 * false => if open failed
 */
bool	open_file(std::string filename)
{
	std::fstream file(filename.c_str());

	if (file.fail())
		return (false);
	return (true);
}

/**
 * @brief check if ip address passed is well formatted
 * 
 * @param addr string that contains ip address
 * @return ip address len if verified, \p `false` if not
 */
int	valid_ip_address(std::string addr)
{
	size_t	dots;
	int		len;
	int		ip;

	dots = 0;
	for (len = 0; addr[len] && addr[len] != ':'; len++)
	{
		if (addr[len] != '.' && std::isdigit(addr[len]) == 0)
			return (false);
		dots += (addr[len] == '.');
	}
	if (dots != 3)
		return (false);
	for (int i = 4; i; i--)
	{
		ip = std::atoi(addr.c_str());
		if (ip > UINT8_MAX || (ip != 0 && addr[0] == '0'))
			return (false);
		while (addr[0] != '.' && addr[0] != ':' && !addr.empty())
			addr.erase(0, 1);
		if (addr[0] && addr[1] && (addr[1] == '.' || addr[1] == ':'))
			return (false);
		addr.erase(0, 1);
	}
	return (len);
}
