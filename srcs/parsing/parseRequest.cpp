#include "../../includes/ether.hpp"
#include "../../includes/status_codes.hpp"
#include "../hpp/Client.hpp"
#include "../hpp/Request.hpp"
#include "../hpp/Server.hpp"

static int	lineParsing(Request &request, std::string line);
static int	headerParsing(Request &request, std::istringstream &header);
static int	bodyParsing(Request &request, std::istringstream &header);
std::string	removeWhitespaces(std::string line);
//static int	errorParsing(Request &request, e_http_codes code);
static int	errorParsing(Request &request, e_http_codes code, std::string info);

int	requestParsing(Request &request, std::string input)
{
	std::string			lines = "\r";
	std::istringstream	s(input);

	while (lines == "\r")
		std::getline(s, lines, '\n');
	if (lineParsing(request, lines) != 0) // first line parsing
		return (request.getStatusCode());
	if (headerParsing(request, s) != 0) // header parsing
		return (request.getStatusCode());
	if (bodyParsing(request, s) != 0)
		return (request.getStatusCode());
	request.setStatusCode(HTTP_OK);
	return (0);
}

// REVIEW Questa funzione va a prendere la prima riga dell'header per stabilire il tipo di 
// connessione e ricava il metodo, l'url dell'oggetto della connessione e la 
// versione http. Questo perchè in base al metodo si va a stabilire il tipo
// di richiesta e quindi i membri che ci aspettiamo di trovare.
// POST /HTTP/1.1
// Host: localhost:9006
// User-Agent: curl/8.17.1-DEV
// Accept: */*
// Content-Length: 10
// Content-Type: application/x-www-form-urlencoded
static int	lineParsing(Request &request, std::string line)
{
	std::string	field;

	if (trim_equal_left(line, ' ') == 1)
		return (errorParsing(request, HTTP_CE_BAD_REQUEST, "Method"));
	field = line;
	trim_diff_right(field, ' ');
//SECTION - method
	request.setMethod(field);
	if (request.getMethod() == UNDEFINED)
		return (errorParsing(request, HTTP_SE_NOT_IMPLEMENTED, "method " \
		+ request.getMethod()));
	trim_diff_left(line, ' ');
	if (!line[0] || !line[1] || trim_equal_left(line, ' ') == 1)
		return (errorParsing(request, HTTP_CE_BAD_REQUEST, "Format"));
	field = line;
	trim_diff_right(field, ' ');
//SECTION - URI
	if (field[0] != '/')
		return (errorParsing(request, HTTP_SE_NOT_IMPLEMENTED, "No origin form"));
	if (field.length() > MAX_URI_LENGTH)
		return (errorParsing(request, HTTP_CE_URI_TOO_LONG, field));
	request.setUrl(field);
	if (request.getUrl().empty() == true)
		return (errorParsing(request, HTTP_CE_BAD_REQUEST, "URI"));
	trim_diff_left(line, ' ');
	if (!line[0] || !line[1] || trim_equal_left(line, ' ') == 1)
		return (errorParsing(request, HTTP_CE_BAD_REQUEST, "Format"));
//SECTION - version
	if (line != "HTTP/1.1\r")
		return (errorParsing(request, HTTP_SE_NOT_IMPLEMENTED, "" \
		+ line));
	request.setHttpVersion("HTTP/1.1");
	return (0);
}

// REVIEW - Questa funzione va a controllare il formato dell'header della 
// richiesta di connessione e si assicura che ci siano tutti i membri necessari in
// in base ai metodi che dobbiamo gestire -GET -POST -DELETE
static int	headerParsing(Request &request, std::istringstream &header)
{
	std::string		line;
	std::string		key;
	size_t			sep;

	request.resetRequest();
	while (std::getline(header, line) && line != "\r") // da trimmare \r
	{
		if (line[0] == '\t')
			return (errorParsing(request, HTTP_CE_BAD_REQUEST, "Line folding is deprecated"));
		if (line.rbegin()[0] != '\r')
			return (errorParsing(request, HTTP_CE_BAD_REQUEST, "Missing \\r"));
		line.erase(line.size() - 1);
		if (line.find_first_of('\r') != std::string::npos)
			return (errorParsing(request, HTTP_CE_BAD_REQUEST, "Invalid \\r"));
		sep = line.find(':');
		if (sep == std::string::npos)
			return (errorParsing(request, HTTP_CE_BAD_REQUEST, "Missing : in head"));
		key = line.substr(0, sep);
		if (find_first_whitespace(key) != key.length())
			return (errorParsing(request, HTTP_CE_BAD_REQUEST, "Key with WS"));
		if (sep + 2 < line.size())
			request.setHeaderVal(key, line.substr(key.length() + 2));
		else
			return (errorParsing(request, HTTP_CE_BAD_REQUEST, "Empty header val"));
	}
	if (!request.checkHeader())
		return (errorParsing(request, HTTP_CE_BAD_REQUEST, "Missing head"));
	return (0);
}

//FIXME - non va letta una nuova linea
static int	bodyParsing(Request &request, std::istringstream &stream)
{
	std::string line;
	std::string	body;

	request.setBodyLen(std::atoi(request.getHeaderVal("Content-Length").c_str()));
	if (!line.empty())
		return(errorParsing(request, HTTP_CE_BAD_REQUEST, "No \\n between header/body"));
	if (request.getHeaderVal("Transfer-Encoding") != "")
		;//FIXME - gestire transfer encoding
	while (std::getline(stream, line, '\n'))
	{
		if (line.rbegin()[0] != '\r')
			return(errorParsing(request, HTTP_CE_BAD_REQUEST, "No \\r in body"));
		body += line;
	}
	switch (request.getMethodEnum())
	{
		case POST :
			if (body.empty())
				;//ERRORE
			if (request.getBodyLen() == 0 && \
			request.getHeaderVal("Transfer-Encoding") != "chunked")
				;//ERRORE
			if (request.getBodyLen() || request.getBodyLen() != body.length())
					;//ERRORE
			break ;
		case GET :
			if (body.empty())
				break ;
			if (request.getBodyLen() == 0 && \
			request.getHeaderVal("Transfer-Encoding") != "chunked")
				;//ERRORE
			if (request.getBodyLen() || request.getBodyLen() != body.length())
				;//ERRORE
		case DELETE ://body e delete???
			if (body.empty())
				break ;
			if (request.getBodyLen() == 0 && \
			request.getHeaderVal("Transfer-Encoding") != "chunked")
				;//ERRORE
			if (request.getBodyLen() || request.getBodyLen() != body.length())
				;//ERRORE
		case HEAD :
		if (!body.empty())
			;//ERRORE
		break ;
		default :
			;//errore
	}
	request.setBody(body);
	return (0);
}

/*
static int	errorParsing(Request &request, e_http_codes code)
{
	return (errorParsing(request, code, ""));
}*/

static int	errorParsing(Request &request, e_http_codes code, std::string info)
{
	int	int_code;

	int_code = code;
	switch (int_code)
	{
		case HTTP_CE_BAD_REQUEST :
			SWITCH_LOG(info, "ClientError: Bad Request");
		case HTTP_CE_UNATHORIZED :
			SWITCH_LOG(info, "ClientError: Unathorized");
		case HTTP_CE_FORBIDDEN:
			SWITCH_LOG(info, "ClientError: Forbidden");
		case HTTP_CE_NOT_FOUND:
			SWITCH_LOG(info, "ClientError: Not found");
		case HTTP_CE_METHOD_NOT_ALLOWED:
			SWITCH_LOG(info, "ClientError: Method not allowed");
		case HTTP_CE_CONFLICT:
			SWITCH_LOG(info, "ClientError: Another process is currently using the requested resource");
		case HTTP_CE_LENGTH_REQUIRED:
			SWITCH_LOG(info, "ClientError: Lenght required");
		case HTTP_CE_URI_TOO_LONG:
			SWITCH_LOG(info, "ClientError: Uri too long");
		case HTTP_CE_MEDIA_TYPE:
			SWITCH_LOG(info, "ClientError: Media type of requested file is unknown");
		case HTTP_CE_IM_TEAPOT:
			SWITCH_LOG(info, "ClientError: Want a cup of tea?" CHARIZARD);
		case HTTP_SE_NOT_IMPLEMENTED:
			SWITCH_LOG(info, "ServerError: Not implemented -->");
		default :
			SWITCH_LOG(info, "ClientError: Wtf" PIEDI_DELLA_ZIA_DEL_TUO_RAGAZZO);	
	}
	request.setStatusCode(code);
	request.setRequestErrorBool(true);
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
