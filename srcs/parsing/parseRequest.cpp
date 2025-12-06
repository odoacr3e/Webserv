
#include "../../includes/ether.hpp"
#include "../hpp/Client.hpp"
#include "../hpp/Request.hpp"

static int	errorParsing(int err, std::string s)
{
	std::cerr << "\033[31m" << s << "\033[0m" << std::endl;
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
	if (request.getMethod() == UNDEFINED)
		return (errorParsing(400, "Bad post\n"));// ERROR : METODO NON RICONOSCIUTO
	request.setUrl(line.substr(method.length() + 1, line.find(' ', method.length() + 1) - (method.length() + 1)));
	if (request.getUrl().empty() == true)
		return (errorParsing(400, "Error in URL\n"));
	request.setHttpVersion(line.substr(method.length() + 1 + \
		request.getUrl().length() + 1, line.find('\n', method.length() + 1 + \
		request.getUrl().length() + 1) - (method.length() + 1) - (request.getUrl().length() + 1)));
	//controlli sul http version
	if (request.getHttpVersion().compare("HTTP/1.1\r") != 0)
		return (errorParsing(400, "Error in HTTP Version\n"));
	return (0);
}

// #define GAY std::string request.getHeader()[line.substr(0, ':')]

/*
Host: localhost:8080
User-Agent: curl/8.17.1-DEV
Accept: /
Content-Length: 10 (SOLO POST)
Content-Type: application/x-www-form-urlencoded (SOLO POST)
*/
int	headerParsing(Request &request, std::istringstream &header)
{
	std::string	line;
	std::string	key;

	request.reset_request();
	request.printHeader();
	while (std::getline(header, line) && line != "\r")
	{
		key = line.substr(0, line.find(':'));
		if (request.getHeader().find(key) != request.getHeader().end())
		{
			request.getHeader()[key] = line.substr(key.length() + 2);
			std::cout << "\033[32m";
			std::cout << key << ": " << request.getHeader()[key] << std::endl;
			std::cout << "\033[0m";
		}
		else
			return (errorParsing(104, (std::string)"Campo header illegale: " + key));
	}
	if (!request.checkVal("Host") || \
		!request.checkVal("Accept") || \
		!request.checkVal("User-Agent"))
		return (errorParsing(104, "Una flag obbligatoria non e definita.\n"));	
	else if (request.getMethod() == "POST")
	{
		if (!request.checkVal("Content-Length") || \
		!request.checkVal("Content-Type"))
			return (errorParsing(104, "Una flag obbligatoria POST non e definita.\n"));
	}
	return (0);
}

// GET METH REQUESTED FIELDS
// GET /contact HTTP/1.1
// Host: example.com
// User-Agent: curl/8.6.0
// Accept: */*

/*
Host: localhost:8080
User-Agent: curl/8.17.1-DEV
Accept: /
Content-Length: 10 (SOLO POST)
Content-Type: application/x-www-form-urlencoded (SOLO POST)


*/
//	METHOD URL VERSIONE_HTTP\r\n
int	requestParsing(Request &request, std::string input)
{
	std::string			lines;
	std::istringstream	s(input);
	int					err;

	std::getline(s, lines, '\n');
	std::cout << request << std::endl;
	if ((err = lineParsing(request, lines)) != 0)
		return (err);
	if ((err = headerParsing(request, s)) != 0)
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


