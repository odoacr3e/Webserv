
#include "../../includes/ether.hpp"
#include "../hpp/Client.hpp"
#include "../hpp/Request.hpp"

static int	errorParsing(int err, std::string s)
{
	std::cerr << "\033[31m" << s << "\033[0m" << std::endl;
	return (err);
}

// REVIEW Questa funzione va a prendere la prima riga dell'header per stabilire il tipo di 
// connessione e ricava il metodo, l'url dell'oggetto della connessione e la 
// versione http. Questo perchè in base al metodo si va a stabilire il tipo
// di richiesta e quindi i membri che ci aspettiamo di trovare.

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
	if (request.getHttpVersion().compare("HTTP/1.1\r") != 0)
		return (errorParsing(400, "Error in HTTP Version\n"));
	return (0);
}

// REVIEW - Questa funzione va a controllare il formato dell'header della 
// richiesta di connessione e si assicura che ci siano tutti i membri necessari in
// in base ai metodi che dobbiamo gestire -GET -POST -DELETE 
int	headerParsing(Request &request, std::istringstream &header)
{
	std::string	line;
	std::string	key;

	request.resetRequest();
	while (std::getline(header, line) && line != "\r") // da trimmare \r
	{
		key = line.substr(0, line.find(':'));
		if (request.getHeader().find(key) != request.getHeader().end())
		{
			request.getHeader()[key] = line.substr(key.length() + 2);
			// std::cout << "\033[32m";
			// std::cout << key << ": " << request.getHeader()[key] << std::endl;
			// std::cout << "\033[0m";
		}
		else
			return (errorParsing(104, (std::string)"Campo header illegale: " + key));
	}
	request.printHeader();
	if (request.checkHeader() == false)
		return (false);
	return (1);
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
	if ((err = lineParsing(request, lines)) != 0)
		return (err);
	std::cout << request << std::endl;
	if ((err = headerParsing(request, s)) == false)
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


