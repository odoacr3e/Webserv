#include "../../hpp/Request.hpp"

//printa tutti gli header in una request
void	Request::printHeader(void)
{
	for (headermap::const_iterator it = _header.begin(); it != _header.end(); it++)
	{
		std::cout << "Key: " << it->first << "\tValue: " << it->second << std::endl;
	}
}

//printa tutto l'oggetto request
std::ostream &operator<<(std::ostream &os, Request &obj)
{
	os << "Method: " << obj.getMethod() << "\nURL: " << \
	obj.getUrl() << "\nVERSION: " << obj.getHttpVersion() << std::endl;
	std::cout << "\033[32m--Request header print--\n" COLOR_RESET;
	obj.printHeader();
	std::cout << "\033[32mStatusCode: " COLOR_RESET << obj.getStatusCode();
	std::cout << std::endl;
	return (os);
}
