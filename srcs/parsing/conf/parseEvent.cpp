#include "../../../includes/ether.hpp"
#include "../../hpp/Conf.hpp"

void		instructionError(std::vector<std::string>&list, int line, std::string s);
void		instructionWarning(std::vector<std::string> &list, int line, std::string s);

void	confParseEvent(Conf &conf, std::vector<std::string> list, int line)
{
	(void)conf, (void)list, (void)line;
	instructionError(list, line, "unrecognized instruction");
}
