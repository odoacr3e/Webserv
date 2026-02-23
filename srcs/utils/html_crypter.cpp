#include "../../includes/ether.hpp"
#include "../../srcs/hpp/Server.hpp"

std::string createHtmlCrypter(t_cgi &cgi_ptr)
{
	std::fstream file("www/cgi-bin/crypter/index.html");
	std::string body;

	body = file_opener(file, "");
	find_and_replace(body, "<!-- {RESULT} -->", cgi_ptr.output);
	return (body);
}