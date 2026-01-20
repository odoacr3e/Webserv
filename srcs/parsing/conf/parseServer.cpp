#include "../../../includes/ether.hpp"
#include "../../hpp/Conf.hpp"

void		instructionError(std::vector<std::string>&list, int line, std::string s);
void		instructionWarning(std::vector<std::string> &list, int line, std::string s);
static void	parseServerName(Conf &conf, std::vector<std::string> list, int line);
static void	parseListen(Conf &conf, std::vector<std::string> list, int line);
static void	parseRoot(Conf &conf, std::vector<std::string> list, int line);
static void	parseIndex(Conf &conf, std::vector<std::string> list, int line);
static void	parseBodySize(Conf &conf, std::vector<std::string> list, int line);
static void	parseErrorPages(Conf &conf, std::vector<std::string> &list, int line);

//NOTE - Allowed server instructions
/*
	-	ServerName
	-	Listen
	-	Root
	-	Index
	-	BodySize
*/
void	confParseServer(Conf &conf, std::vector<std::string> list, int line)
{
	if (list[0] == "server_name")
		parseServerName(conf, list, line);
	else if (list[0] == "listen")
		parseListen(conf, list, line);
	else if (list[0] == "root")
		parseRoot(conf, list, line);
	else if (list[0] == "index")
		parseIndex(conf, list, line);
	else if (list[0] == "client_max_body_size")
		parseBodySize(conf, list, line);
	else if (list[0] == "error_page")
		parseErrorPages(conf, list, line);
	else
		instructionError(list, line, "unrecognized instruction");
}

static void	parseServerName(Conf &conf, std::vector<std::string> list, int line)
{
	size_t	size;

	size = list.size();
	if (size == 1)
		instructionError(list, line, "empty instruction");
	for (size_t i = 1; i < size; i++)
		conf.addServerName(list[i]);
}

static std::string checkListenIp(std::vector<std::string> list, int line, std::string &ip)
{
	std::string	ip_port;
	int			ip_len = 0;

	ip_port = list[1];
	if (ip_port[0] == '*')
		ip = "0.0.0.0";
	else if ((ip_len = valid_ip_address(ip_port)) != 0)
		ip = ip_port.substr(0, ip_len);
	else if (ip_port.find(':') == std::string::npos)
		ip = DEFAULT_CONF_IP;
	if (ip_port[0] == '*' && ip_port[1] != ':')
		instructionWarning(list, line, "wildcard ip addr needs a : separator");
	if (ip_len)//
		ip_port = ip_port.substr(ip_len);//
	if (ip_port[0] == '*'  || ip_port[0] == ':')
		ip_port.erase(0, 1  + (ip_port[0] == '*' && ip_port[1] == ':') );
	return (ip_port);
}

static int checkListenPort(std::vector<std::string> list, int line, std::string ip_port)
{
	int			port = DEFAULT_CONF_PORT;

	if (!ip_port.empty())
		port = std::atoi(ip_port.c_str());
	if (!ip_port.empty() && ip_port.find_first_not_of("0123456789") != std::string::npos)
		instructionError(list, line, "listen syntax violated");
	return (port);
}

static void	parseListen(Conf &conf, std::vector<std::string> list, int line)
{
	std::string	ip_port;
	std::string	ip;
	int			port;

	if (list.size() > 2)
		instructionError(list, line, "listen does not manage flags (such as ssl)");
	else if (list.size() != 2)
		instructionError(list, line, "empty instruction");
	ip_port = checkListenIp(list, line, ip);
	port = checkListenPort(list, line, ip_port);
	conf.setIpPort(ip, port);
	conf.getServerBlock().listen_set = true;
}

static void	parseRoot(Conf &conf, std::vector<std::string> list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "please give one valid path");
	if (conf.getServerBlock().root.empty() == false)
		instructionWarning(list, line, "root already defined. Old is replaced");
	if (valid_directory(list[1]) == false)
		instructionError(list, line, "not a valid path");
	conf.getServerBlock().root = list[1];
}

static void	parseIndex(Conf &conf, std::vector<std::string> list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "please give one valid path");
	if (conf.getServerBlock().index.empty() == false)
		instructionWarning(list, line, "index already defined. Old is replaced");
	conf.getServerBlock().index = list[1];
}

static void	parseBodySize(Conf &conf, std::vector<std::string> list, int line)
{
	int	body_size;

	if (list.size() != 2)
		instructionError(list, line, "please give one valid integer");
	if (conf.getServerBlock().client_max_body_size != 0)
		instructionWarning(list, line, "body_size already defined");
	if (!std::strchr("0123456789", list[1][0]))
		instructionError(list, line, "body size must be a valid number");
	body_size = std::atoi(list[1].c_str());
	if (body_size < 0)
		instructionError(list, line, "negative body size.. seriously man??");
	if (body_size == 0)
		instructionWarning(list, line, "body size 0 will be normalized");
	conf.getServerBlock().client_max_body_size = body_size;
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
	if (conf.getServerBlock().err_pages.count(code) > 0)
		instructionError(list, line, "error page for this status code already set");
	conf.getServerBlock().err_pages[code] = list[2];
}
