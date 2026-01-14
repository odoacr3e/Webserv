#include "../../../includes/ether.hpp"
#include "../../hpp/Conf.hpp"

void		instructionError(std::vector<std::string>&list, int line, std::string s);
void		instructionWarning(std::vector<std::string> &list, int line, std::string s);
static void	parseAlias(Conf &conf, std::vector<std::string> &list, int line);
static void	parseRoot(Conf &conf, std::vector<std::string> &list, int line);
static void	parsePPass(Conf &conf, std::vector<std::string> list, int line);

//NOTE - Allowed location instructions
/*`
	//FIXME - aggiungere
*/
void	confParseLocation(Conf &conf, std::vector<std::string> list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "wrong parameters number for instruction");
	if (list[0] == "alias")
		parseAlias(conf, list, line);
	else if(list[0] == "root")
		parseRoot(conf, list, line);
	else if(list[0] == "proxy_pass")
		parsePPass(conf, list, line);
	else
		instructionError(list, line, "unrecognized instruction");
}

static void	parseAlias(Conf &conf, std::vector<std::string> &list, int line)
{
	if (valid_directory(list[1]) == false)
		instructionError(list, line, "invalid alias path");
	if (*list[1].rbegin() != '/')
		instructionError(list, line, "alias requires / at the end");
	conf.getLocationBlock().alias = list[1];
}

static void	parseRoot(Conf &conf, std::vector<std::string> &list, int line)
{
	if (valid_directory(list[1]) == false)
		instructionError(list, line, "invalid root path");
	conf.getLocationBlock().root = list[1];

}

static void	parsePPass(Conf &conf, std::vector<std::string> list, int line)
{
	(void)conf, (void)list, (void)line;
}
