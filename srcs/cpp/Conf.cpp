
#include "../hpp/Conf.hpp"

Conf::Conf(std::string filepath): _file(filepath)
{
	std::ifstream fd(filepath.c_str(), std::ios_base::in);

	this->_events = false;
	this->_http = false;
	this->_server = false;
	this->_location = false;
	if (fd.fail())
		throw ConfException("Invalid configuration file");
	confParse(*this, fd);
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

bool	Conf::getEvents() const
{
	return (this->_events);
}

bool	Conf::getHttp() const
{
	return (this->_http);
}

bool	Conf::getServer() const
{
	return (this->_server);
}

bool	Conf::getLocation() const
{
	return (this->_location);
}

void	Conf::setEvents(bool val)
{
	this->_events = val;
}

void	Conf::setHttp(bool val)
{
	this->_http = val;
}

void	Conf::setServer(bool val)
{
	this->_server = val;
}

void	Conf::setLocation(bool val)
{
	this->_location = val;
}

std::string	Conf::checkOpenBlock(void) const
{
	std::cout << "http: " << this->_http << ", server: " << this->_server << ", location: " << this->_location << std::endl;
	if (this->_events)
		return ("events");
	else if (this->_http && !this->_server && !this->_location)
		return ("http");
	else if (this->_server && this->_http && !this->_location)
		return ("server");
	else if (this->_location && this->_server && this->_location)
		return ("location");
	return ("");
}

// utils

void	Conf::print(void) const
{
	std::cout << "\033[35mPrint of all configurations:\n";
	std::cout << "\033[33m{MAIN BLOCK}\n";
	if (this->_user.empty() == false)
		std::cout << "\033[34mUser:\t\033[33m" << this->_user << "\n";
	std::cout << "\033[0m";
}

// main block
std::string	Conf::getMainUser(void) const
{
	return (this->_user);
}

void		Conf::setMainUser(std::string user)
{
	this->_user = user;
}
