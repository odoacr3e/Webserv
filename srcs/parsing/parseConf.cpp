
#include "../../includes/ether.hpp"
#include "../hpp/Conf.hpp"

void	confParseEvent(Conf &conf, std::vector<std::string> list, int line);
void	confParseHttp(Conf &conf, std::vector<std::string> list, int line);
void	confParseServer(Conf &conf, std::vector<std::string> list, int line);
void	confParseLocation(Conf &conf, std::vector<std::string> list, int line);
void	confParseMain(Conf &conf, std::vector<std::string> list, int line);

static void	blockError(std::string block, int line, int flag)
{
	std::string	error;
	std::string	error2;

	error = "ConfException in line \033[33m" + ft_to_string(line);
	error2 = "ConfException:\033[33m";
	if (flag == CONF_BLOCK_CLOSE)
		throw Conf::ConfException(error + ": cannot close " + block + "\033[0m");
	else if (flag == CONF_BLOCK_FORMAT)
		throw Conf::ConfException(error + ": invalid block format\033[0m");
	else if (flag == CONF_BLOCK_EMPTY)
		throw Conf::ConfException(error + ": block is empty, lol\033[0m");
	else if (flag == CONF_INSTRUCTION_UNFINISHED)
		throw Conf::ConfException(error + ": missing ; before block end\033[0m");
	else if (flag == CONF_INSTRUCTION_EMPTY)
		throw Conf::ConfException(error + ": instruction is empty\033[0m");
	else if (flag == CONF_BLOCK_OPEN)
		throw Conf::ConfException(error + ": block opened into another block\033[0m");
	else if (flag == CONF_MULT_BLOCK)
		throw Conf::ConfException(error + ": multiple block " + block + "\033[0m");
	else if (flag == CONF_MISSING_BLOCK)
		throw Conf::ConfException(error2 + " missing " + block + " in configuration file\033[0m");
	else if (flag == CONF_PATH_INVALID)
		throw Conf::ConfException(error + ": " + block + " does not exist!\033[0m");
	if (block.compare("events") && block.compare("http") && block.compare("server") && block.compare("location"))
		error += ": " + block + " is not allowed (allowed: events, http, server, location)";
	else
		error += " block order violated.";
	throw Conf::ConfException(error);
}

/*
	-	controlla che la lista di istruzioni non sia vuota.
	-	viene chiamata una funzione in base al tipo di blocco corrente
	-	la lista di istruzioni viene svuotata
*/
static int	instructionBlock(Conf &conf, std::vector<std::string> &list, int i)
{
	if (list.size() < 1)
		blockError(" ", i, CONF_INSTRUCTION_EMPTY);
	else if (conf.getEvents())
		confParseEvent(conf, list, i);
	else if (conf.getHttp() && !conf.getServer() && !conf.getLocation())
		confParseHttp(conf, list, i);
	else if (conf.getServer() && conf.getHttp() && !conf.getLocation())
		confParseServer(conf, list, i);
	else if (conf.getLocation() && conf.getHttp() && conf.getServer())
		confParseLocation(conf, list, i);
	else
		confParseMain(conf, list, i);
	list.clear();
	return (true);
}

static int	openBlock(Conf &conf, std::vector<std::string> &list, int line)
{
	if (list.size() > 1UL + (list[0] == "location"))
		blockError(list[0], line, CONF_BLOCK_FORMAT);
	else if (list.size() < 1)
		blockError("", line, CONF_BLOCK_EMPTY);
	else if ((list[0] == "events" && conf.getBlockNumber(conf.B_EVENTS) > 0) || \
		(list[0] == "http" && conf.getBlockNumber(conf.B_HTTP) > 0))
		blockError(list[0], line, CONF_MULT_BLOCK);
	if (list[0] == "events" && !conf.getHttp() && !conf.getEvents())
	{
		conf.setEvents(true);
		conf.updateBlock(conf.B_EVENTS);
	}
	else if (list[0] == "http" && conf.getBlockNumber(conf.B_HTTP) == 0 && !conf.getHttp() && !conf.getEvents())
	{
		conf.setHttp(true);
		conf.updateBlock(conf.B_HTTP);
	}
	//server dentro ad http non gia aperto e fuori da location o da events
	else if (list[0] == "server" && conf.getHttp() && !conf.getServer() && !conf.getLocation() && !conf.getEvents())
	{
		conf.setServer(true);
		conf.updateBlock(conf.B_SERVER);
	}
	else if (list[0] == "location" && conf.getServer() && conf.getHttp() && !conf.getLocation() && !conf.getEvents())
	{
		if (!valid_directory(list[1]))
			blockError(list[1], line, CONF_PATH_INVALID);
		conf.setCurrLocation(list[1]);
		conf.setLocation(true);
	}
	else
		blockError(list[0], line, CONF_BLOCK_INVALID);
	list.clear();
	return (1);
}

/*
	- 	chiude il blocco corrente.
	-	se viene chiuso un blocco server, 
		i suoi dati vengono pushati nel vector<t_conf_server>
	-	se non ci sono blocchi da chiudere, errore
*/
static int	closeBlock(Conf &conf, int line)
{
	if (conf.getEvents())
		conf.setEvents(false);
	else if (conf.getHttp() && conf.getServer() && conf.getLocation() && !conf.getEvents())
	{
		conf.getServerBlock().location[conf.getCurrLocation()] = conf.getLocationBlock();
		conf.setLocation(false);
	}
	else if (conf.getHttp() && conf.getServer() && !conf.getLocation() && !conf.getEvents())
	{
		conf.getServerBlock().set_if_empty();
		conf.getConfServer().push_back(conf.getServerBlock());
		conf.setServer(false);
	}
	else if (conf.getHttp() && !conf.getServer() && !conf.getLocation() && !conf.getEvents())
		conf.setHttp(false);
	else
		blockError("", line, CONF_BLOCK_CLOSE);
	return (1);
}

void	instructionError(std::vector<std::string> &list, int line, std::string s)
{
	std::string	error;

	error = "\033[31mConfException: in line " + ft_to_string(line);
	error += ", instruction \033[34m";
	for (size_t i = 0; i < list.size(); i++)
		error += list[i] + " ";
	error += "\033[31m\b:\n" + s + "\033[0m";
	throw Conf::ConfException(error);
}

void	instructionWarning(std::vector<std::string> &list, int line, std::string s)
{
	std::string	error;

	error = "\033[31mConfWarning: in line " + ft_to_string(line);
	error += ", instruction \033[34m";
	for (size_t i = 0; i < list.size(); i++)
		error += list[i] + " ";
	error += "\033[31m\b:\n" + s + "\033[0m";
	std::cerr << error << std::endl;
}

//ANCHOR - confParse
/*
	Read configuration file. Parse it. Saves data in Conf class.

	@special:	throw exceptions
	@input:		[Conf &conf]-------------->	reference to conf class.
				[std::ifstream &fd]------->	reference to conf file stream.
	@return:	none 
	@var:	
		[std::string line]-------------->	current line read
		[std::string token]------------->	current token.
											tokens are separated by:
											-	ISSPACE (see ascii)
											-	#	comments sign
											-	;	instruction end sign
											-	{	open block sign
											-	}	close block sign
		[std::vector<std::string> list]->	list of all tokens.
											When a token separator is found,
											list is reset.
*/
void	confParse(Conf &conf, std::ifstream &fd)
{
	std::string					line;
	std::string					token;
	std::vector<std::string>	list;
	int i = 0;

	std:: cout << "Conf: \033[35mPrint of all token:\n";
	while (std::getline(fd, line))
	{
		i++;
		while (!line.empty())
		{
			line = removeWhitespaces(line);
			if (line[0] == ';')
				instructionBlock(conf, list, i);
			else if (line[0] == '{')
				openBlock(conf, list, i);
			else if (line[0] == '}')
				closeBlock(conf, i);
			else if (line[0] == '#')
				break ;
			else 
				token = line.substr(0, find_first_special_char(line));
			if (!token.empty())
				list.push_back(token);
			line = line.substr(find_first_special_char(line));
			if (token.empty() == false)
				std::cout << "\033[34mCurrent token:\t\033[33m" << token << "\033[0m\n";
			token = "";
		}
	}
	if (!conf.missingBlock().empty())
		blockError(conf.missingBlock(), -1, CONF_MISSING_BLOCK);
	if (list.size() != 0) // per istruzioni senza apertura o senza ;
		blockError("", i, CONF_INSTRUCTION_UNFINISHED);
	if (conf.getEvents() || conf.getHttp() || conf.getServer() || conf.getLocation())
		blockError(conf.checkOpenBlock(), i, CONF_BLOCK_CLOSE);
	std::cout << conf << std::endl;
}
