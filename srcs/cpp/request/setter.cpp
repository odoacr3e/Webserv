#include "../../hpp/Request.hpp"

/*NOTE - index of content
		-	method
		-	url
		-	HttpVersion
		-	Headers
		-	Body
		-	Status code, error bool
*/

//SECTION - method

void	Request::setMethod(std::string method)
{
	this->_method = UNDEFINED;
	if (method.length() > this->_max_method_length)
		return ;
	for (int i = 0; i < this->getMethNum(); i++)
	{
		if (method == this->_validmethods[i])
		{
			this->_method = method;
			return ;
		}
	}
}

//SECTION - url

void	Request::setUrl(std::string url)
{
	if (url.find('\r') != std::string::npos)
	{
		this->_url = "";
		return ;
	}
	this->_url = url;
}

//SECTION - HttpVersion

void	Request::setHttpVersion(std::string version)
{
	this->_http_version = version;
}

//SECTION - Headers

bool	Request::setHeaderVal(std::string key, std::string val)
{
	// std::cout << "\033[33mNEW:" COLOR_RESET << key << " " << val << "\n";
	if (key == "Content-Length" && !charFinder(val, std::isdigit))
		return (1);
	this->_header[key] = val;
	return (0);
}

//SECTION - body

void	Request::setBody(std::string body)
{
	this->_body = body;
}

void	Request::setBodyLen(size_t len)
{
	this->_body_len = len;
}

void 	Request::setBodyType(std::string type)
{
	this->_body_type = type;
}

//SECTION - Status code, error bool

void	Request::setStatusCode(e_http_codes status_code)
{
	this->_status_code = status_code;
}

void	Request::setRequestErrorBool(bool error)
{
	this->_error = error;
	if (this->_error)
		DBG_MSG("SERVER NOT FOUND\n");
}
