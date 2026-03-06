#include "../../hpp/Request.hpp"

//printa tutti gli header in una request
void	Request::printHeader(void)
{
	for (headermap::const_iterator it = _header.begin(); it != _header.end(); it++)
	{
		LOG_TERM << "Key: " << it->first << "\tValue: " << it->second << std::endl;
	}
}

//printa tutto l'oggetto request
std::ostream &operator<<(std::ostream &os, Request &obj)
{
	os << "Method: " << obj.getMethod() << "\nURL: " << \
	obj.getUrl() << "\nVERSION: " << obj.getHttpVersion() << std::endl;
	LOG_TERM << "\033[32m--Request header print--\n" COLOR_RESET;
	obj.printHeader();
	LOG_TERM << "\033[32mStatusCode: " COLOR_RESET << obj.getStatusCode();
	LOG_TERM << std::endl;
	return (os);
}
