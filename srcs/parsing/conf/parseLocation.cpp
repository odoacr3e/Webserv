#include "../../../includes/ether.hpp"
#include "../../hpp/Conf.hpp"

void		instructionError(std::vector<std::string>&list, int line, std::string s);
void		instructionWarning(std::vector<std::string> &list, int line, std::string s);
static void	parseAlias(Conf &conf, std::vector<std::string> &list, int line);
static void	parseRoot(Conf &conf, std::vector<std::string> &list, int line);
static void parseCgiParam(Conf &conf, std::vector<std::string> &list, int line);
static void parseReturn(Conf &conf, std::vector<std::string> &list, int line);

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
	else if (list[0] == "autoindex")
		conf.getLocationBlock().autoindex = true;
	else
		instructionError(list, line, "unrecognized instruction");
}

static void	parseAlias(Conf &conf, std::vector<std::string> &list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "wrong parameters number for alias instruction");
	if (valid_directory(list[1]) == false)
		instructionError(list, line, "invalid alias path");
	if (*list[1].rbegin() != '/')
		instructionError(list, line, "alias must finish with /");
	conf.getLocationBlock().alias = list[1];
}

static void	parseRoot(Conf &conf, std::vector<std::string> &list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "wrong parameters number for root instruction");
	if (valid_directory(list[1]) == false)
		instructionError(list, line, "invalid root path");
	conf.getLocationBlock().root = list[1];
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

/*
return code [text]
return code URL
return URL
return code
*/
bool charFinder(const std::string &literal, int(*check)(int))
{
	int index;
	int end;

	index = 0;
	end = literal.size();
	
	while (!literal.empty() && index < end)
	{
		if (check(literal[index]) == false)
			return (false);
		index++;
	}
	return (true);
}

static void parseReturn(Conf &conf, std::vector<std::string> &list, int line)
{
	int code = std::atoi(list[1].c_str());

	if (list.size() < 2 || list.size() > 3)
		instructionError(list, line, "return needs this syntax:	return code [text] |"\
		"return code URL | return URL | return code\n");
	if (list.size() == 2)// URL o code
	{
		if (!code)
		{
			if (!valid_directory(list[2]))
				conf.getCurrLocation();
			
		}	//error
			;//verifica uri
		// check se e uri
		//se si -> inserisco dati
		// se no -> e un code? atoi fallisce?
		// se fallisce -> errore
	}
	else
	{
		if (code >= 300 && code < 309)
			instructionError(list, line, "return code for redirect must be between 300 and 308\n");
		conf.getLocationBlock().ret.first = code;
		conf.getLocationBlock().ret.second = list[2];
	}
}
