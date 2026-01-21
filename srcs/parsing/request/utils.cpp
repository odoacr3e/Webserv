#include "../../../includes/ether.hpp"
#include "../../../includes/status_codes.hpp"
#include "../../hpp/Client.hpp"
#include "../../hpp/Request.hpp"

int	errorParsing(Request &request, e_http_codes code, std::string info);
int	errorParsing(Request &request, e_http_codes code);

bool	bodyChecker(Request &request, std::string &body, bool accept_empty)
{
	if (body.empty())
	{
		if (accept_empty)
			return (0);
		return (errorParsing(request, HTTP_CE_BAD_REQUEST, "missing body"));
	}
	if (request.getBodyLen() == 0 && \
	request.getHeaderVal("Transfer-Encoding") != "chunked")
		return (errorParsing(request, HTTP_CE_LENGTH_REQUIRED));
	if (request.getBodyLen() && request.getBodyLen() != body.length())
		return (errorParsing(request, HTTP_CE_BAD_REQUEST, "invalid body len"));
	return (0);
}
