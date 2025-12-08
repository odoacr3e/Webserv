
#include "../hpp/Conf.hpp"

Conf::Conf(std::string filepath): _file(filepath)
{
	std::ifstream fd(filepath.c_str(), std::ios_base::in);

	if (fd.fail())
		throw ConfException("Invalid configuration file");
	parseMain(*this, fd);
}

Conf::~Conf()
{}

Conf::Conf(const Conf &other)
{
	*this = other;
}

Conf	&Conf::operator=(const Conf &other)
{
	if (this == &other)
		return (*this);
	return (*this);
}

// void	Conf::validOpenBlock(std::string toFlag) const
// {
	
// }