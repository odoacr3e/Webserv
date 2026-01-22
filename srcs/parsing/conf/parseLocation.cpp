#include "../../../includes/ether.hpp"
#include "../../hpp/Conf.hpp"

void		instructionError(std::vector<std::string>&list, int line, std::string s);
void		instructionWarning(std::vector<std::string> &list, int line, std::string s);
static void	parseAlias(Conf &conf, std::vector<std::string> &list, int line);
static void	parseRoot(Conf &conf, std::vector<std::string> &list, int line);
static void parseCgiParam(Conf &conf, std::vector<std::string> &list, int line);
static void parseReturn(Conf &conf, std::vector<std::string> &list, int line);
static void	parseIndex(Conf &conf, std::vector<std::string> list, int line);
static void	parseAutoindex(Conf &conf, std::vector<std::string> &list, int line);
static void	parseErrorPages(Conf &conf, std::vector<std::string> &list, int line);

//NOTE - Allowed location instructions
/*
	//FIXME - aggiungere
*/
void	confParseLocation(Conf &conf, std::vector<std::string> list, int line)
{
	if (list[0] == "alias")
		parseAlias(conf, list, line);
	else if(list[0] == "root")
		parseRoot(conf, list, line);
	else if (list[0] == "fastcgi_param")
		parseCgiParam(conf, list, line);
	else if (list[0] == "return")
		parseReturn(conf, list, line);
	else if (list[0] == "index")
		parseIndex(conf, list, line);
	else if (list[0] == "autoindex")
		parseAutoindex(conf, list, line);
	else if (list[0] == "error_page")
		parseErrorPages(conf, list, line);
	else
		instructionError(list, line, "unrecognized instruction");
}

static void	parseAlias(Conf &conf, std::vector<std::string> &list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "wrong parameters number for alias instruction");
	if (!conf.getLocationBlock().root.empty())
		instructionError(list, line, "invalid parameter \"alias\": root already exists");
	if (valid_directory(list[1]) == false)
		instructionError(list, line, "invalid alias path");
	if (*list[1].rbegin() != '/')
		instructionError(list, line, "alias must finish with /");
	if (*(list[1].rbegin()) == '/' && *(conf.getCurrLocation().rbegin()) != '/')
		instructionError(list, line, "alias can be present only with uri ending with '/'");
	conf.getLocationBlock().alias = list[1];
}

static void	parseRoot(Conf &conf, std::vector<std::string> &list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "wrong parameters number for root instruction");
	if (!conf.getLocationBlock().alias.empty())
		instructionError(list, line, "invalid parameter \"root\": alias already exists");
	if (valid_directory(list[1]) == false)
		instructionError(list, line, "invalid root path");
	if (list[1].rbegin()[0] == '/')
		list[1].erase(list[1].length() - 1);
	conf.getLocationBlock().root = list[1];
}

static void	parseIndex(Conf &conf, std::vector<std::string> list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "please give one valid path");
	if (conf.getLocationBlock().index.empty() == false)
		instructionWarning(list, line, "index already defined. Old is replaced");
	conf.getLocationBlock().index = list[1];
}

static void parseCgiParam(Conf &conf, std::vector<std::string> &list, int line)
{
	size_t	type_size;
	size_t	path_size;

	if (list.size() != 3)
		instructionError(list, line, "wrong parameters number for fastcgi_params instruction");
//		.cgi	/path.cgi
//		1)	controllare che sia estensione valida (eseguibile compilato, python, php)
	if (list[1] != ".php" && list[1] != ".py" && list[1] != "exec")
		instructionError(list, line, "invalid CGI extension");
//		2)	controllare che path finisca con cgi
	type_size = list[1].size();
	path_size = list[2].size();
	if (path_size < type_size)
		instructionError(list, line, "invalid CGI path");
	if (list[1] != list[2].substr(path_size - type_size, type_size))
		instructionError(list, line, "CGI extension doesn't match path");
//		3)	controllare che path esista
	if (valid_file(list[2]) == false)
		instructionError(list, line, "invalid CGI path");
	std::pair<std::string, std::string> p(list[1], list[2]);
	conf.getLocationBlock().cgiparam.push_back(p);
}

static void parseReturn(Conf &conf, std::vector<std::string> &list, int line)
{
	bool	code_syntax;
	int		code;
	int		code_valid;

	if (list.size() < 2 || list.size() > 3)
		instructionError(list, line, "return needs this syntax:	return code [text] |"\
		"return code URL | return URL | return code\n");
	code_syntax = charFinder(list[1], std::isdigit);
	code = std::atoi(list[1].c_str());
	code_valid = checkValidCode(code);
	if (list.size() == 2) //URL o code
	{
		if (!code_syntax)
		{
			conf.getLocationBlock().ret_uri = list[1];
			conf.getLocationBlock().ret_code = 302;
		}
		else if (code_valid == HTTP_UNKNOWN)
			instructionError(list, line, "invalid status code\n");
		else if (code >= 300 && code < 399)
			instructionError(list, line, "return code 3xx must have an uri\n");
		else
			conf.getLocationBlock().ret_code = code;
		return ;
	}
	if (!code_syntax)
		instructionError(list, line, "Wrong status code syntax\n");
	if (code_valid == HTTP_UNKNOWN)
		instructionError(list, line, "invalid status code\n");
	conf.getLocationBlock().ret_code = code;
	if (code >= 300 && code < 399)
	{
		if (list[2][0] != '/')
			instructionError(list, line, "return code 3xx must have an uri\n");
		conf.getLocationBlock().ret_uri = list[2];
	}
	else
		conf.getLocationBlock().ret_text = list[2];
}

static void	parseAutoindex(Conf &conf, std::vector<std::string> &list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "autoindex usage: autoindex <on | off>");
	else if (list[1] == "on")
		conf.getLocationBlock().autoindex = true;
	else if (list[1] == "off")
		conf.getLocationBlock().autoindex = false;
	else
		instructionError(list, line, "autoindex usage: autoindex <on | off>");
}

/*
error_page code ... [=[response]] uri
default:
 
context: http, server, location, if in location
 
Defines the URI that will be shown for the specified errors. error_page directives are inherited from the previous level only if there are no error_page directives defined on the current level. A uri value can contain variables. Example:

error_page 404             /404.html;
Furthermore, it is possible to change the response code to another using the =response syntax, for example:

If an error response is processed by a proxied server or a FastCGI server, and the server may return different response codes (e.g., 200, 302, 401 or 404), it is possible to respond with the code it returns:

It is also possible to use redirects for error processing:

error_page 403      http://example.com/forbidden.html;
error_page 404 =301 http://example.com/notfound.html;
In this case, by default, the response code 302 is returned to the client. It can only be changed to one of the redirect status codes (301, 302, 303, and 307). If there is no need to change URI during internal redirection it is possible to pass error processing into a named location:

*/
static void	parseErrorPages(Conf &conf, std::vector<std::string> &list, int line)
{
	if (list.size() != 3)
		instructionError(list, line, "bad error pages params");
	bool code_syntax = charFinder(list[1], std::isdigit);
	if (!code_syntax)
		instructionError(list, line, "invalid status code format in error_pages param");
	int		code = std::atoi(list[1].c_str());
	int		code_valid = checkValidCode(code);
	if (code_valid == HTTP_UNKNOWN)
		instructionError(list, line, "status code not implemented");
	if (code >= 300 && code <= 399)
		instructionError(list, line, "redirect are invalid in error pages");
	if (list[2][0] != '/')
		instructionError(list, line, "Uri must start with /");
	if (conf.getLocationBlock().err_pages.count(code) > 0)
		instructionError(list, line, "error page for this status code already set");
	conf.getLocationBlock().err_pages[code] = list[2];
}
