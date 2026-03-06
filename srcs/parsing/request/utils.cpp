#include "../../../includes/include.hpp"
#include "../../../includes/status_codes.hpp"
#include "../../hpp/Client.hpp"
#include "../../hpp/Request.hpp"

/// @brief checks the headers that refers to body
/// @param request reference to request
/// @param body reference to request body
/// @param accept_empty method accepts valid body?
/// @return 0 if ok, 1 if error occured
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

/// @brief used to trim and get data from html first line 
///			GET /photo/dogs HTTP/1.1
/// @param line first request line
/// @param field reference to the result of the trim
/// @return 0 if ok, 1 if error occured
bool	getNextFirstLineField(std::string &line, std::string &field)
{
	if (!line[0] || !line[1] || trim_equal_left(line, ' ') == 1)
	{
		LOG_TERM << "line 0: " << line[0] << ", line 1: " << std::endl;
		return (1);
	}
	field = line;
	trim_diff_left(line, ' ');
	trim_diff_right(field, ' ');
	return (0);
}
