#include "../../includes/ether.hpp"
#include "../hpp/Client.hpp"
#include "../hpp/Request.hpp"

static int	lineParsing(Request &request, std::string line);
static int	headerParsing(Request &request, std::istringstream &header);
static int	bodyParsing(Request &request, std::istringstream &header);
std::string	removeWhitespaces(std::string line);
static int	errorParsing(int err, std::string s);

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
	if ((err = bodyParsing(request, s)) == false)
		return (err);
	return (0);
}

// REVIEW Questa funzione va a prendere la prima riga dell'header per stabilire il tipo di 
// connessione e ricava il metodo, l'url dell'oggetto della connessione e la 
// versione http. Questo perchè in base al metodo si va a stabilire il tipo
// di richiesta e quindi i membri che ci aspettiamo di trovare.
static int	lineParsing(Request &request, std::string line)
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
		return (errorParsing(400, "Bad request"));// ERROR : METODO NON RICONOSCIUTO
	request.setUrl(line.substr(method.length() + 1, line.find(' ', method.length() + 1) - (method.length() + 1)));
	if (request.getUrl().empty() == true)
		return (errorParsing(400, "Bad request\n"));
	request.setHttpVersion(line.substr(method.length() + 1 + \
		request.getUrl().length() + 1, line.find('\n', method.length() + 1 + \
		request.getUrl().length() + 1) - (method.length() + 1) - (request.getUrl().length() + 1)));
	if (request.getHttpVersion().compare("HTTP/1.1\r") != 0)
		return (errorParsing(400, "Bad request\n"));
	return (0);
}

// REVIEW - Questa funzione va a controllare il formato dell'header della 
// richiesta di connessione e si assicura che ci siano tutti i membri necessari in
// in base ai metodi che dobbiamo gestire -GET -POST -DELETE
static int	headerParsing(Request &request, std::istringstream &header)
{
	std::string		line;
	std::string		key;

	request.resetRequest();
	while (std::getline(header, line) && line != "\r") // da trimmare \r
	{
		key = line.substr(0, line.find(':'));
		request.setHeaderVal(key, line.substr(key.length() + 2));
	}
	if (!request.checkHeader())
		return (false);
	return (true);
}

//FIXME - non va letta una nuova linea
static int	bodyParsing(Request &request, std::istringstream &header)
{
	std::string line;

	if (!line.empty())
		return(errorParsing(400, "Bad format request\n"));
	if (request.getMethod().compare("POST") == 0 && \
		std::atoi(request.getHeaderVal("Content-Length").c_str()) > 0)
	{
		std::getline(header, line);
		if (line.empty())
			return(errorParsing(405, "Method not allowed\n"));
		request.setBody(line);
	}
	return (200);
}

static int	errorParsing(int err, std::string s)
{
	std::cerr << "\033[31m" << s << "\033[0m" << std::endl;
	return (err);
}


// POST /contact HTTP/1.1
// Host: example.com
// User-Agent: curl/8.6.0
// Accept: */*
// \r\n
// User-Agent: curl/8.6.0

// POST /contact HTTP/1.1
// Host: example.com
// User-Agent: curl/8.6.0
// Accept: */*
// \r\n

// POST /contact HTTP/1.1
// Host: example.com
// \r\n
// User-Agent: curl/8.6.0
// Accept: */*
// \r\n
