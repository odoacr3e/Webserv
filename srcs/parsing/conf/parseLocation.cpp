#include "../../../includes/ether.hpp"
#include "../../hpp/Conf.hpp"

void		instructionError(std::vector<std::string>&list, int line, std::string s);
void		instructionWarning(std::vector<std::string> &list, int line, std::string s);
static void	parseAlias(Conf &conf, std::vector<std::string> &list, int line);
static void	parseRoot(Conf &conf, std::vector<std::string> &list, int line);
static void parseCgiPath(Conf &conf, std::vector<std::string> &list, int line);
static void parseReturn(Conf &conf, std::vector<std::string> &list, int line);
static void	parseIndex(Conf &conf, std::vector<std::string> list, int line);
static void	parseScriptBool(Conf &conf, std::vector<std::string> list, int line);
static void	parseScriptType(Conf &conf, std::vector<std::string> list, int line);
static void	parseAutoindex(Conf &conf, std::vector<std::string> &list, int line);
static void	parseErrorPages(Conf &conf, std::vector<std::string> &list, int line);
static void	parseMethodsList(Conf &conf, std::vector<std::string> &list, int line);
static void	parseStorage(Conf &conf, std::vector<std::string> &list, int line);
static void	parseCookie(Conf &conf, std::vector<std::string> &list, int line);

//NOTE - Allowed location instructions
void	confParseLocation(Conf &conf, std::vector<std::string> list, int line)
{
	if (list[0] == "alias")
		parseAlias(conf, list, line);
	else if(list[0] == "root")
		parseRoot(conf, list, line);
	else if (list[0] == "cgi_path")
		parseCgiPath(conf, list, line);
	else if (list[0] == "return")
		parseReturn(conf, list, line);
	else if (list[0] == "index")
		parseIndex(conf, list, line);
	else if (list[0] == "autoindex")
		parseAutoindex(conf, list, line);
	else if (list[0] == "error_page")
		parseErrorPages(conf, list, line);
	else if (list[0] == "methods")
		parseMethodsList(conf, list, line);
	else if (list[0] == "storage")
		parseStorage(conf, list, line);
	else if (list[0] == "script" || list[0] == "script_daemon")
		parseScriptBool(conf, list, line);
	else if (list[0] == "script_type")
		parseScriptType(conf, list, line);
	else if (list[0] == "cookie")
		parseCookie(conf, list, line);	
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
	conf.getLocationBlock().root = url_normalize(list[1]);
	conf.getLocationBlock().alias = true;
}

static void	parseRoot(Conf &conf, std::vector<std::string> &list, int line)
{
	std::string	url;
	bool		slash;

	if (list.size() != 2)
		instructionError(list, line, "wrong parameters number for root instruction");
	if (!conf.getLocationBlock().root.empty())
		instructionError(list, line, "invalid parameter \"root\": root already exists");
	if (valid_directory(list[1]) == false)
		instructionError(list, line, "invalid root path");
	slash = conf.getCurrLocation()[0] == '/';
	url = list[1] + (conf.getCurrLocation().c_str() + slash);
	if (valid_directory(url) == false)
		instructionError(list, line, url + " is not valid");
	conf.getLocationBlock().root = url_normalize(list[1]);
}

static void	parseIndex(Conf &conf, std::vector<std::string> list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "please give one valid path");
	if (conf.getLocationBlock().index.empty() == false)
		instructionWarning(list, line, "index already defined. Old is replaced");
	conf.getLocationBlock().index = list[1];
}

bool	valid_exec(std::string file);

static void parseCgiPath(Conf &conf, std::vector<std::string> &list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "cgi_path takes 1 arg");
	if (valid_exec(list[1]) == false)
		instructionError(list, line, "invalid CGI path");
	conf.getLocationBlock().cgi_path = list[1];
}

static void parseReturn(Conf &conf, std::vector<std::string> &list, int line)
{
	bool	code_syntax;
	int		code;
	int		code_valid;

	if (list.size() < 2)
		instructionError(list, line, "return needs this syntax:	return code [text] |"\
		"return code3XX URL | return code\n");
	code_syntax = charFinder(list[1], std::isdigit);
	code = std::atoi(list[1].c_str());
	code_valid = checkValidCode(code);
	if (!code_syntax)
		instructionError(list, line, "Wrong status code syntax\n");
	if (code_valid == HTTP_UNKNOWN)
		instructionError(list, line, "invalid status code\n");
	conf.getLocationBlock().ret_code = code;
	if (code >= 300 && code < 399)
	{
		if (list.size() != 3 || list[2][0] != '/')
			instructionError(list, line, "return code 3xx must have an uri\n");
	}
	for (size_t i = 2; i != list.size(); i++)
		conf.getLocationBlock().ret_text += list[i] + ' ';
}

static void	parseAutoindex(Conf &conf, std::vector<std::string> &list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "autoindex usage: autoindex <on | off>");
	else if (list[1] == "on" || list[1] == "true")
		conf.getLocationBlock().autoindex = true;
	else if (list[1] == "off" || list[1] == "false")
		conf.getLocationBlock().autoindex = false;
	else
		instructionError(list, line, "autoindex usage: autoindex <on | off>");
}

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

static void	parseMethodsList(Conf &conf, std::vector<std::string> &list, int line)
{
	if (list.size() < 2)
		instructionError(list, line, "bad methods params");
	if (conf.getLocationBlock().mask_methods != MASK_NO_METHODS)
		instructionError(list, line, "methods already set");
	for (size_t i = 1; i != list.size(); i++)
	{
		if (list[i] == "GET")
			conf.getLocationBlock().mask_methods |= MASK_GET;
		else if (list[i] == "POST")
			conf.getLocationBlock().mask_methods |= MASK_POST;
		else if (list[i] == "DELETE")
			conf.getLocationBlock().mask_methods |= MASK_DELETE;
		else if (list[i] == "HEAD")
			conf.getLocationBlock().mask_methods |= MASK_HEAD;
		else if (list[i] == "ALL")
			conf.getLocationBlock().mask_methods = MASK_ALL_METHODS;
		else if (list[i] == "NONE")
			conf.getLocationBlock().mask_methods = MASK_NO_METHODS;
		else
			instructionError(list, line, "unrecognized method");
	}
}

static void	parseStorage(Conf &conf, std::vector<std::string> &list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "bad storage path param");
	if (valid_directory(list[1]))
		conf.getLocationBlock().post_storage = list[1];
	else
		instructionError(list, line, "storage path not valid");	
}

static void	parseScriptBool(Conf &conf, std::vector<std::string> list, int line)
{
	bool	*ptr;

	if (list.size() != 2)
		instructionError(list, line, "bad script params number");
	if (list[0] == "script")
		ptr = &conf.getLocationBlock().run_script;
	else if (list[0] == "script_daemon")
		ptr = &conf.getLocationBlock().script_daemon;
	else
		instructionError(list, line, "unrecognized ScriptBool");
	if (list[1] == "on")
		*ptr = true;
	else if (list[1] == "off")
		*ptr = false;
	else
		instructionError(list, line, "script accepts on \"or\" \"off\" only");
}

static void	parseScriptType(Conf &conf, std::vector<std::string> list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "bad script_type params number");
	conf.getLocationBlock().script_type = list[1];
}

static void	parseCookie(Conf &conf, std::vector<std::string> &list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "cookie format: cookie on|off");
	if (list[1] == "on")
		conf.getLocationBlock().gen_cookie = true;
	else if (list[1] == "off")
		conf.getLocationBlock().gen_cookie = false;
	else
		instructionError(list, line, "cookie format: cookie on|off");
}
