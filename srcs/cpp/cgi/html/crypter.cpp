#include "../../../hpp/Cgi.hpp"

typedef std::vector<char *>	argvVector;

std::string createHtmlCrypter(t_cgi &cgi_ptr)
{
	std::fstream file("www/cgi-bin/crypter/index.html");
	std::string body;
	// char *new

	std::cout.clear();
	body = file_opener(file, "");
	// cgi_ptr.output;
	std::cout << "RESULT: " << "|" << cgi_ptr.output << "|" << std::endl;
	find_and_replace(body, "<!-- {RESULT} -->", cgi_ptr.output);
	return (body);
}
