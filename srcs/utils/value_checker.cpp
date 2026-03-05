#include "../../includes/ether.hpp"


//On success: 	returns the length of the ip address.
//On failure:	returns 0.
int	valid_ip_address(std::string addr)
{
	size_t	dots;
	int		len;
	int		ip;
	// size_t	separator_index;

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
		// NOTE - da togliere commento sotto se vogliamo ascoltare solo su 127.x.x.x
		if (/* (i == 4 && ip != 127) || */ ip > UINT8_MAX || (ip != 0 && addr[0] == '0'))
			return (false);
		while (addr[0] != '.' && addr[0] != ':' && !addr.empty())
			addr.erase(0, 1);
		// separator_index = addr.find_first_of(".:", 1);
	//	if (addr[0] == '.' && separator_index > 3 && separator_index != addr.npos)
	//		return (false);
		if (addr[0] && addr[1] && (addr[1] == '.' || addr[1] == ':'))
			return (false);
		addr.erase(0, 1);
	}
	return (len);
}

//if s is not a valid_ip, crash
int	atohex(std::string s)
{
	int	hex = 0;

	for (size_t i = 0;;)
	{
		std::cout << s.substr(i) << std::endl;
		hex = hex * 16 * 16 + std::atoi(s.substr(i).c_str());
		i = s.find(".", i);
		if (i == s.npos)
			break ;
		i++;
	}
	return (hex);
}

// listen 127.0..:9090