#include "../../includes/ether.hpp"

//On success: 	returns the length of the ip address.
//On failure:	returns 0.
int	valid_ip_address(std::string addr)
{
	size_t	dots;
	int		len;

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
		if (std::atoi(addr.c_str()) > UINT8_MAX)
			return (false);
		while (addr[0] != '.' && addr[0] != ':' && !addr.empty())
			addr.erase(0, 1);
		addr.erase(0, 1);
	}
	return (len);
}
