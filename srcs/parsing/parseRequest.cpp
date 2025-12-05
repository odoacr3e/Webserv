
#include "../../includes/ether.hpp"
#include "../hpp/Client.hpp"
#include "../hpp/Request.hpp"

static int	errorParsing(int err, std::string s)
{
	std::cerr << s << std::endl;
	return (err);
}

//GET /index.html HTTP/1.1
int	lineParsing(Request &request, std::string line)
{
	std::string	method;
	int			i;

	method = line.substr(0, line.find(' '));
	for (i = 0; i < request.getMethNum(); i++)
	{
		if (method == request.getValidMethod(i))
			request.setMethod(i);
	}
	std::cout << "METODO: " << request.getMethod() << std::endl;
	std::cout << "i: " << i << std::endl;
	if (request.getMethod() == UNDEFINED)
		return (errorParsing(400, "Bad post\n")); // ERROR : METODO NON RICONOSCIUTO
	std::cout << "Line find space: " << line.find(' ', method.length() + 1) - (method.length() + 1) << std::endl;
	std::cout << "line index start url: " << method.length() + 1 << std::endl;
	request.setUrl(line.substr(method.length() + 1, line.find(' ', method.length() + 1) - (method.length() + 1)));
	//controlli sulla url
	if (request.getUrl().empty() == true)
		return (errorParsing(400, "Empty request\n"));
	request.setHttpVersion(line.substr(method.length() + 1 + \
	request.getUrl().length() + 1, line.find('\n')));
	//controlli sul http version
	if (!request.getHttpVersion().compare("HTTP/1.1\r"))
		return (errorParsing(400, "Empty request\n"));
	return (0);
}

// int	headerParsing(Request &request, std::istringstream header);

//	METHOD URL VERSIONE_HTTP\r\n
int	requestParsing(Request &request, std::string input)
{
	std::string			lines;
	std::istringstream	s(input);
	int					err;

	std::getline(s, lines, '\n');
	err = lineParsing(request, lines);
	std::cout << request << std::endl;
	if (err)
		return (err);
	return (0);
}


// POST /api/v1/messages HTTP/1.1\r\nHost: esempio.com\nUser-Agent: curl/8.5.0\nAccept: */*\nContent-Type: application/json\nContent-Length: 63

// POST /api/v1/messages HTTP/1.1\r\n
// Host: esempio.com\r\n
// User-Agent: curl/8.5.0\r\n
// Accept: */*\r\n
// Content-Type: application/json\r\n
// Content-Length: 63\r\n
// \r\n
// {"to":"+391234567890","from":"MAGICO","text":"Messaggio test"}