
#include "../../includes/ether.hpp"
#include "../hpp/Conf.hpp"

//NOTE - parsing/parseConf.cpp

void		confParse(Conf &conf, std::ifstream &fd);
static int	instructionBlock(Conf &conf, std::vector<std::string> &list, int i);
static void	openBlock(Conf &conf, std::vector<std::string> &list, int line);
static void	closeBlock(Conf &conf, std::vector<std::string> &list, int line);
void		instructionError(std::vector<std::string> &list, int line, std::string s);
void		instructionWarning(std::vector<std::string> &list, int line, std::string s);
static void	blockError(std::string block, int line, int flag);

//NOTE - parsing/conf/parse*.cpp

void		confParseEvent(Conf &conf, std::vector<std::string> list, int line);
void		confParseHttp(Conf &conf, std::vector<std::string> list, int line);
void		confParseServer(Conf &conf, std::vector<std::string> list, int line);
void		confParseLocation(Conf &conf, std::vector<std::string> list, int line);
void		confParseMain(Conf &conf, std::vector<std::string> list, int line);

//ANCHOR - confParse
/*
Read configuration file. Parse it. Saves data in Conf class.

@input:		[Conf &conf]--->	reference to conf class.
			[ifstream &fd]->	reference to conf file stream.
@ret:	none 
@variables:	
[string line]-------------->	current line read
[string token]------------->	current token.
								tokens are separated by:
									-	ISSPACE (see ascii)
									-	#	comments sign
									-	;	instruction end sign
									-	{	open block sign
									-	}	close block sign
[vector<std::string> list]->	list of all tokens.
								When a token separator is found,
								list is processed and reset.
*/
void	confParse(Conf &conf, std::ifstream &fd)
{
	std::string					line;
	std::string					token;
	std::vector<std::string>	list;
	int i = 0;

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
				closeBlock(conf, list, i);
			else if (line[0] == '#')
				break ;
			else 
				token = line.substr(0, find_first_special_char(line));
			if (!token.empty())
				list.push_back(token);
			line = line.substr(find_first_special_char(line));
			token = "";
		}
	}
	if (!conf.missingBlock().empty())
		blockError(conf.missingBlock(), -1, CONF_MISSING_BLOCK);
	if (list.size() != 0) // per istruzioni senza apertura o senza ;
		blockError("", i, CONF_INSTRUCTION_UNFINISHED);
	if (conf.getBlockType() != Conf::B_NONE)
		blockError(conf.checkOpenBlock(), i, CONF_BLOCK_UNIFINISHED);
}

/*NOTE - instructionBlock
	-	controlla che la lista di istruzioni non sia vuota.
	-	viene chiamata una funzione in base al tipo di blocco corrente.
	-	la lista di istruzioni viene svuotata.
*/
static int	instructionBlock(Conf &conf, std::vector<std::string> &list, int i)
{
	if (list.size() < 1)
		blockError(" ", i, CONF_INSTRUCTION_EMPTY);
	switch (conf.getBlockType())
	{
		case (Conf::B_EVENTS) :
			confParseEvent(conf, list, i);
			break ;
		case (Conf::B_HTTP) :
			confParseHttp(conf, list, i);
			break ;
		case (Conf::B_SERVER) :
			confParseServer(conf, list, i);
			break ;
		case (Conf::B_LOCATION) :
			confParseLocation(conf, list, i);
			break ;
		default :
			confParseMain(conf, list, i);
	}
	list.clear();
	return (true);
}

/*
NOTE - openBlock
	-	controlla che la lista di istruzioni:
		- NON abbia size() == 0.
		- se blocco location: size == 2.
		- se altri blocchi: size == 1.
	-	Apre il blocco richiesto, se possibile.
	-	I setter chiamano i costruttori di 
		t_conf_server e t_conf_location.
	-	la lista di istruzioni viene svuotata.
*/
static void	openBlock(Conf &conf, std::vector<std::string> &list, int line)
{
	if (list.size() == 0)
		blockError("unnamed block", line, CONF_BLOCK_EMPTY);
	if (list.size() > 1UL + (list[0] == "location"))
		blockError(list[0], line, CONF_BLOCK_FORMAT);
	switch (conf.checkBlockType(list[0]))
	{
		case (Conf::B_EVENTS) :
			conf.setEvents(true);
			break ;
		case (Conf::B_HTTP) :
			conf.setHttp(true);
			break ;
		case (Conf::B_SERVER) :
			conf.setServer(true);
			break ;
		case (Conf::B_LOCATION) :
			if (!valid_directory(list[1]))
				blockError(list[1], line, CONF_PATH_INVALID);
			if (conf.getServerBlock().location.count(list[1]) > 0)
				blockError(list[1], line, CONF_MULT_LOCATION);
			conf.setLocation(true, list[1]);
			break ;
		case (Conf::B_NONE) :
			return (blockError(list[0], line, CONF_BLOCK_INVALID));
		case (CONF_MULT_BLOCK) :
			return (blockError(list[0], line, CONF_MULT_BLOCK));
		case (CONF_BLOCK_OPEN) :
			return (blockError(list[0], line, CONF_BLOCK_OPEN));
	}
	list.clear();
}

/*NOTE - closeBlock
	- 	chiude il blocco corrente.
	-	i dati del blocco corrente vengono salvati.
		- LOCATION: setta dati non impostati di location. 
		- SERVER: setta dati non impostati di location e server. 
	-	se non ci sono blocchi da chiudere, errore.
*/
static void	closeBlock(Conf &conf, std::vector<std::string> &list, int line)
{
	if (list.size() != 0)
		blockError("", line, CONF_INSTRUCTION_UNFINISHED);
	switch (conf.getBlockType())
	{
		case (Conf::B_EVENTS) :
			conf.setEvents(false);
			break ;
		case (Conf::B_HTTP) :
			conf.setHttp(false);
			break ;
		case (Conf::B_LOCATION) :
			conf.getLocationBlock().set_if_empty(conf);
			conf.getServerBlock().location[conf.getCurrLocation()] = conf.getCopyLocationBlock();
			conf.setLocation(false, "");
			break ;
		case (Conf::B_SERVER) :
			conf.getLocationBlock().set_if_empty(conf);
			conf.getServerBlock().set_if_empty(conf);
			conf.getConfServer().push_back(conf.getServerBlock());
			//conf.getLocationBlock().set("");
			conf.setServer(false);
			break ;
		default :
			blockError("", line, CONF_BLOCK_CLOSE);
	}
}

// NOTE - throw exception for instructions errors
void	instructionError(std::vector<std::string> &list, int line, std::string s)
{
	std::string	error;

	error = "\033[31mConfException: in line " + ft_to_string(line);
	error += ", instruction \033[34m";
	for (size_t i = 0; i < list.size(); i++)
		error += list[i] + " ";
	error += "\033[31m\b: " + s + "\033[0m";
	throw Conf::ConfException(error);
}

// NOTE - print instructions errors without exception
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

// NOTE - throw exception for block errors
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
	else if (flag == CONF_BLOCK_UNIFINISHED)
		throw Conf::ConfException(error + ": " + block + " block never closed\033[0m");
	else if (flag == CONF_INSTRUCTION_UNFINISHED)
		throw Conf::ConfException(error + ": missing ; before block end\033[0m");
	else if (flag == CONF_INSTRUCTION_EMPTY)
		throw Conf::ConfException(error + ": instruction is empty\033[0m");
	else if (flag == CONF_BLOCK_OPEN)
		throw Conf::ConfException(error + ": block cannot be opened\033[0m");
	else if (flag == CONF_MULT_BLOCK)
		throw Conf::ConfException(error + ": multiple block " + block + "\033[0m");
	else if (flag == CONF_MISSING_BLOCK)
		throw Conf::ConfException(error2 + " missing " + block + " in configuration file\033[0m");
	else if (flag == CONF_PATH_INVALID)
		throw Conf::ConfException(error + ": " + block + " does not exist!\033[0m");
	else if (flag == CONF_MULT_LOCATION)
		throw Conf::ConfException(error + "path location already exist\033[0m");
	else if (block != "events" && block != "http" && \
	block != "server" && block != "location")
		error += ": " + block + " is not allowed (allowed: events, http, server, location)\033[0m";
	else
		error += " block order violated.\033[0m";
	throw Conf::ConfException(error);
}

//NOTE - old istruction block function
/*
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
*/

//NOTE - old open block function
/*
{
	if (list.size() == 0)
		blockError("unnamed block", line, CONF_BLOCK_EMPTY);
	if (list.size() > 1UL + (list[0] == "location"))
		blockError(list[0], line, CONF_BLOCK_FORMAT);
	if ((list[0] == "events" && conf.getBlockNumber(conf.B_EVENTS) > 0) || \
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
		conf.getServerBlock().set();
		conf.updateBlock(conf.B_SERVER);
	}
	else if (list[0] == "location" && conf.getServer() && conf.getHttp() && !conf.getLocation() && !conf.getEvents())
	{
		if (!valid_directory(list[1]))
			blockError(list[1], line, CONF_PATH_INVALID);
		conf.setCurrLocation(list[1]);
		conf.getLocationBlock().set(list[1]);
		conf.setLocation(true);
	}
	else
		blockError(list[0], line, CONF_BLOCK_INVALID);
	list.clear();
	return (1);
}
*/
