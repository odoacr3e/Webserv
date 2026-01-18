#include "../../includes/ether.hpp"
#include "../../includes/status_codes.hpp"
#include "../hpp/Client.hpp"
#include "../hpp/Request.hpp"

static int	lineParsing(Request &request, std::string line);
static int	headerParsing(Request &request, std::istringstream &header);
static int	bodyParsing(Request &request, std::istringstream &header);
std::string	removeWhitespaces(std::string line);
static int	errorParsing(Request &request, e_http_codes code);
static int	errorParsing(Request &request, e_http_codes code, std::string info);

int	requestParsing(Request &request, std::string input)
{
	std::string			lines;
	std::istringstream	s(input);

	std::getline(s, lines, '\n');
	if (lineParsing(request, lines) != 0)
		return (request.getStatusCode());
	if (headerParsing(request, s) != 0)
		return (request.getStatusCode());
	if (bodyParsing(request, s) != 0)
		return (request.getStatusCode());
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
		return (errorParsing(request, HTTP_CE_BAD_REQUEST, "no method"));
	request.setUrl(line.substr(method.length() + 1, line.find(' ', method.length() + 1) - (method.length() + 1)));
	if (request.getUrl().empty() == true)
		return (errorParsing(request, HTTP_CE_BAD_REQUEST, "No uri"));
	request.setHttpVersion(line.substr(method.length() + 1 + \
		request.getUrl().length() + 1, line.find('\n', method.length() + 1 + \
		request.getUrl().length() + 1) - (method.length() + 1) - (request.getUrl().length() + 1)));
	if (request.getHttpVersion().compare("HTTP/1.1\r") != 0)
		return (errorParsing(request, HTTP_CE_BAD_REQUEST, "Bad http version"));
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
		if (line.rbegin()[0] != '\r')
			return (errorParsing(request, HTTP_CE_BAD_REQUEST, "missing \\r"));
		line.erase(line.size() - 1);
		if (line.find_first_of('\r') != std::string::npos)
			return (errorParsing(request, HTTP_CE_BAD_REQUEST, "invalid \\r"));
		std::cout << "LINES :" << line << std::endl;
		key = line.substr(0, line.find(':'));
		request.setHeaderVal(key, line.substr(key.length() + 2));
	}
	if (!request.checkHeader())
		return (1);
	return (0);
}

//FIXME - non va letta una nuova linea
static int	bodyParsing(Request &request, std::istringstream &header)
{
	std::string line;

	if (!line.empty())
		return(errorParsing(request, HTTP_CE_BAD_REQUEST, "No body"));
	if (request.getMethod().compare("POST") == 0 && \
		std::atoi(request.getHeaderVal("Content-Length").c_str()) > 0)
	{
		std::getline(header, line);
		if (line.empty())
			return(errorParsing(request, HTTP_CE_METHOD_NOT_ALLOWED));
		request.setBody(line);
	}
	return (0);
}

static int	errorParsing(Request &request, e_http_codes code)
{
	return (errorParsing(request, code, ""));
}

static int	errorParsing(Request &request, e_http_codes code, std::string info)
{
	int	int_code;

	int_code = code;
	switch (int_code)
	{
		case HTTP_CE_BAD_REQUEST :
			SWITCH_LOG(info, "Http ClientError: Bad Request");
	}
	request.setStatusCode(code);
	return (1);
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
