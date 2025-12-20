#include "../../../includes/ether.hpp"
#include "../../hpp/Conf.hpp"

void		instructionError(std::vector<std::string>&list, int line, std::string s);
void		instructionWarning(std::vector<std::string> &list, int line, std::string s);
static void	parseServerName(Conf &conf, std::vector<std::string> list, int line);
static void	parseListen(Conf &conf, std::vector<std::string> list, int line);
static void	parseRoot(Conf &conf, std::vector<std::string> list, int line);
static void	parseIndex(Conf &conf, std::vector<std::string> list, int line);
static void	parseBodySize(Conf &conf, std::vector<std::string> list, int line);

//NOTE - Allowed server instructions
/*
	//FIXME - aggiungere gestione parametri (vedi struttura.md)
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
	else
	{
		std::cerr << "\033[31mConf server, line " << line << ":\t\
		unrecognized instruction:\t" << list[0] << "\n\033[0m";
	}
}

/*
//NOTE - 	i controlli sono stati inseriti per evitare che 
			nel conf file uno dei nomi del server sia stato
			già definito.
			Non so se sia giusto accertarcene.
*/
static void	parseServerName(Conf &conf, std::vector<std::string> list, int line)
{
	size_t	size;

	size = list.size();
	if (size == 1)
		instructionError(list, line, "empty instruction");
	for (size_t i = 1; i < size; i++)
	{
		if (conf.findServerName(list[i]))
			instructionError(list, line, "Server name already defined");
		conf.addServerName(list[i]);
	}
}

/*
//NOTE - da testare:
	-	Casi normali:
	listen 127.0.0.1:8000;
	listen 127.0.0.1;
	listen 8000;
	listen *:8000;
//FIXME - non gestisce:
	-	Casi normali:
	listen 127.0.0.1:8000;
	listen 127.0.0.1;
	listen 8000;
	listen *:8000;
	-	IPv6 addresses
	listen [::]:8000;
	listen [::1];
	-	DNS
	listen unix:/var/run/nginx.sock;
	listen localhost:8000;
*/
static void	parseListen(Conf &conf, std::vector<std::string> list, int line)
{
	std::string	ip;
	std::string	ip_port;
	int			ip_len;

	if (list.size() > 2)
		instructionWarning(list, line, "listen does not manage flags, such as ssl");
	else if (list.size() != 2)
		instructionError(list, line, "empty instruction");
	ip_port = list[1];
	ip_len = 0;
	if (ip_port[0] == '*')//SECTION - gestione ip address
		ip = "0.0.0.0";
	else if ((ip_len = valid_ip_address(ip_port)) != 0)
		ip = ip_port.substr(0, ip_len);
	else if (ip_port.find(':') == std::string::npos)
		ip = DEFAULT_CONF_IP;
	if (ip_port[0] == '*' && ip_port[1] != ':')
		instructionWarning(list, line, "wildcard ip addr needs a : separator");
	if (ip_len)//SECTION - rimozione caratteri
		ip_port = ip_port.substr(ip_len);
	if (ip_port[0] == '*')
		ip_port.erase(0, 1);
	if (ip_port[0] == ':')
		ip_port.erase(0, 1);//SECTION - mappatura ip - porta
	if (conf.getServerBlock().ipports.find(ip) != conf.getServerBlock().ipports.end())
		instructionWarning(list, line, "ip addr already set for this server");
	if (ip_port.empty())
		conf.getServerBlock().ipports[ip] = DEFAULT_CONF_PORT;
	else
		conf.getServerBlock().ipports[ip] = std::atoi(ip_port.c_str());
	if (ip_port.find_first_not_of("0123456789") != std::string::npos)
		instructionError(list, line, "listen syntax violated");
	std::cout << "\033[35mparseListen, line " << line << ": added " << ip << ":" << \
	conf.getServerBlock().ipports[ip] << "\n\033[0m";
}

static void	parseRoot(Conf &conf, std::vector<std::string> list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "empty instruction");
	if (conf.getServerBlock().root.empty() == false)
		instructionWarning(list, line, "root already defined. Old is replaced");
	if (valid_directory(list[1]) == false)
		instructionError(list, line, "not a valid path");
	conf.getServerBlock().root = list[1];
}

static void	parseIndex(Conf &conf, std::vector<std::string> list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "empty instruction");
	if (conf.getServerBlock().index.empty() == false)
		instructionWarning(list, line, "index already defined. Old is replaced");
	conf.getServerBlock().index = list[1];
}

static void	parseBodySize(Conf &conf, std::vector<std::string> list, int line)
{
	int	body_size;

	if (list.size() != 2)
		instructionError(list, line, "empty instruction");
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
