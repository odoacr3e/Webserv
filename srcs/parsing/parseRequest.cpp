#include "../../includes/ether.hpp"
#include "../../includes/status_codes.hpp"
#include "../hpp/Client.hpp"
#include "../hpp/Request.hpp"
#include "../hpp/Server.hpp"

static int	lineParsing(Request &request, std::string line);
int			headerParsing(Request &request, bool reset);
static int	bodyParsing(Request &request);
bool		bodyChecker(Request &request, std::string &body, bool accept_empty);
bool		getNextFirstLineField(std::string &line, std::string &field);
std::string	removeWhitespaces(std::string line);
int			executePost(Request &request);
void		print_bin(std::string filename, char *bin_data, size_t len);

int	requestParsing(Client &client, char *input, int bytes)
{
	std::string			lines = "\r";
	Request				&request = client.getRequest();
	std::istringstream	*stream = &request.getRequestStream();

	request.setParsingData(client.getSockFd(), bytes, input);
	while (lines == "\r")//NOTE - linee vuote iniziali accettate da RFC
		std::getline(*stream, lines, '\n');
	if (lineParsing(request, lines) != 0) // first line parsing
		return (request.getStatusCode());
	if (headerParsing(request, true) != 0) // header parsing
		return (request.getStatusCode());
	if (bodyParsing(request) != 0)
		return (request.getStatusCode());
	request.setStatusCode(HTTP_OK);
	return (0);
}

/*REVIEW
	Questa funzione va a prendere la prima riga dell'header per stabilire 
	il tipo di connessione.
	ricava il metodo, l'url dell'oggetto della connessione e la versione http.
	Questo perchè in base al metodo si va a stabilire il tipo
	di richiesta e quindi i membri che ci aspettiamo di trovare.
*/
static int	lineParsing(Request &request, std::string line)
{
	std::string	field;
//-F "image=@/percorso/della/tua/foto.jpg"
	if (std::isspace(line[0]) != 0)
		return (request.fail(HTTP_CE_BAD_REQUEST, "First line format"));
	if (getNextFirstLineField(line, field) == 1)
		return (request.fail(HTTP_CE_BAD_REQUEST, "Method Format"));
//SECTION - method
	request.setMethod(field);
	if (request.getMethod() == UNDEFINED)
		return (request.fail(HTTP_SE_NOT_IMPLEMENTED, "method " + field));
	if (getNextFirstLineField(line, field) == 1)
		return (request.fail(HTTP_CE_BAD_REQUEST, "Url Format"));
//SECTION - URI
	if (field[0] != '/')
		return (request.fail(HTTP_SE_NOT_IMPLEMENTED, "No origin form"));
	if (field.length() > MAX_URI_LENGTH)
		return (request.fail(HTTP_CE_URI_TOO_LONG, field));
	request.setUrl(field);
	if (request.getUrl().empty() == true)
		return (request.fail(HTTP_CE_BAD_REQUEST, "URI"));
	if (getNextFirstLineField(line, field) == 1)
		return (request.fail(HTTP_CE_BAD_REQUEST, "HTTP version Format"));
//SECTION - version
	if (line != "HTTP/1.1\r")
		return (request.fail(HTTP_SE_NOT_IMPLEMENTED, "version " + line));
	request.setHttpVersion("HTTP/1.1");
	return (0);
}

/*REVIEW - 
	Questa funzione va a controllare il formato dell'header della 
	richiesta di connessione e si assicura che ci siano tutti i membri necessari in
	in base ai metodi che dobbiamo gestire -GET -POST -DELETE -HEAD
*/
int			headerParsing(Request &request, bool reset)
{
	std::string		line;
	std::string		key;
	size_t			sep;

	if (reset == true)
		request.resetRequest();
	while (std::getline(request.getRequestStream(), line) && line != "\r") // da trimmare \r
	{
		//SECTION - Key
		if (line[0] == '\t')
			return (request.fail(HTTP_CE_BAD_REQUEST, "Line folding is deprecated"));
		if (line.rbegin()[0] != '\r')
			return (request.fail(HTTP_CE_BAD_REQUEST, "Missing \\r"));
		line.erase(line.length() - 1);
		if (line.find_first_of('\r') != std::string::npos)
			return (request.fail(HTTP_CE_BAD_REQUEST, "Invalid \\r"));
		sep = line.find(':');
		if (sep == std::string::npos)
			return (request.fail(HTTP_CE_BAD_REQUEST, "Missing : in head"));
		key = line.substr(0, sep);
		if (find_first_whitespace(key) != key.length())
			return (request.fail(HTTP_CE_BAD_REQUEST, "Key with WS"));
		//SECTION - val
		line = line.substr(key.length() + 1);
		line = removeWhitespaces(line);
		if (line.empty())
			return (request.fail(HTTP_CE_BAD_REQUEST, "Empty header val"));
		else if (request.setHeaderVal(key, line) == 1)
			return (request.fail(HTTP_CE_BAD_REQUEST, line));
	}
//NOTE - se presenti sia Cont-Len e T-Encoding, togliere T-encoding
	if (request.getBodyLen() != 0 && \
	request.getHeaderVal("Transfer-Encoding") == "chunked")
		return(request.fail(HTTP_CE_BAD_REQUEST, "Conflicting body length and transfer encoding"));
//NOTE - controlli sui nostri header
	if (request.checkHeader() == 1)
		return (1);
//NOTE - Assegnazione della key boundary (\r finale e -- iniziali aggiunti manualmente)
	if (request.getHeaderVal("Content-Type").find("boundary=") != std::string::npos)
	{
		size_t		start = request.getHeaderVal("Content-Type").find("boundary=") + 9;
		// std::cout << "headerParsing() FIND RESULT: " << start << "Value: " << request.getHeaderVal("Content-Type")[start] << std::endl;
		std::string temp = request.getHeaderVal("Content-Type").substr(start);
		request.setHeaderVal("Boundary", temp);
		// std::cout << "headerParsing(): BOUNDARY: " << request.getHeaderVal("Boundary");
	}
	return (line != "\r");//NOTE - se linea diversa da \r, torna errore
}

//FIXME - non va letta una nuova linea
static int	bodyParsing(Request &request)
{
	std::string	body;

	request.setBodyLen(std::atoi(request.getHeaderVal("Content-Length").c_str()));
	if (request.getHeaderVal("Transfer-Encoding") != "")
		{;}//FIXME - gestire transfer encoding
	if (request.getMethodEnum() != POST)
		std::getline(request.getRequestStream(), body, '\0');
	request.setBody(body);
	// std::cout << "bodyParsing()::INITbytesLeft=" << request.getBytesLeft() << "\n";
	request.getBytesLeft() = request.getBodyLen();
	// std::cout << "bodyParsing()::bytesLeft=" << request.getBytesLeft() << "\n";
	switch (request.getMethodEnum())
	{
		case POST :
			if (bodyHeaderParsing(request) == true)//NOTE - aggiungo questa cosa anche in parseRequest
			{
				request.getBinBody().insert(request.getBinBody().end(), request.getSockBuff(), request.getSockBuff() + request.getSockBytes());
				request.getBytesLeft() -= request.getSockBytes();
			}
			return (0);
		case GET :
			return (bodyChecker(request, body, true));
		case DELETE :
			if (!body.empty())
				return (request.fail(HTTP_CE_BAD_REQUEST, "delete must have empty body"));
			return (0);
		case HEAD :
			if (!body.empty())
				return (request.fail(HTTP_CE_BAD_REQUEST, "head must have empty body"));
			return (0);
		case METH_NUM :
			return (request.fail(HTTP_CE_BAD_REQUEST, "BodyParsing: no method"));
	}
	return (0);
}


