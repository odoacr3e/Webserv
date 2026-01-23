#include "../../../includes/ether.hpp"
#include "../../../includes/status_codes.hpp"
#include "../../hpp/Client.hpp"
#include "../../hpp/Request.hpp"

bool	bodyChecker(Request &request, std::string &body, bool accept_empty)
{
	if (body.empty())
	{
		if (accept_empty)
			return (0);
		return (request.fail(HTTP_CE_BAD_REQUEST, "missing body"));
	}
	if (request.getBodyLen() == 0 && \
	request.getHeaderVal("Transfer-Encoding") != "chunked")
		return (request.fail(HTTP_CE_LENGTH_REQUIRED));
	if (request.getBodyLen() && request.getBodyLen() != body.length())
		return (request.fail(HTTP_CE_BAD_REQUEST, "invalid body len"));
	return (0);
}

bool	getNextFirstLineField(std::string &line, std::string &field)
{
	trim_diff_left(line, ' ');
	if (!line[0] || !line[1] || trim_equal_left(line, ' ') == 1)
		return (1);
	field = line;
	trim_diff_right(field, ' ');
	return (0);
}
