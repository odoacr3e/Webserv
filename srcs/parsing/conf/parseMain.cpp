
#include "../../../includes/ether.hpp"
#include "../../hpp/Conf.hpp"

void	instructionError(std::vector<std::string> &list, int line, std::string s);

static void	userInstruction(Conf &conf, std::vector<std::string> list, int line)
{
	if (list.size() != 2)
		instructionError(list, line, "user requires 1 arg");
	conf.setMainUser(list[1]);
}

void	confParseMain(Conf &conf, std::vector<std::string> list, int line)
{
	if (list[0] == "user")
		userInstruction(conf, list, line);
	else
		std::cerr << "\033[31mConf main, line " << line << ":\t\
		unrecognized instruction:\t" << list[0] << "\n\033[0m";
}
