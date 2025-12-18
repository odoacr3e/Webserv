
#include "../hpp/Conf.hpp"

Conf::Conf(std::string filepath): _file(filepath)
{
	std::ifstream fd(filepath.c_str(), std::ios_base::in);

	this->_events = false;
	this->_http = false;
	this->_server = false;
	this->_location = false;
	this->_nevents = 0;
	this->_nhttp = 0;
	this->_nserver = 0;
//	std::memset(&this->_srvblock, 0, sizeof(this->_srvblock));
	std::memset(&this->_locblock, 0, sizeof(this->_locblock));
	if (fd.fail())
		throw ConfException("Invalid configuration file");
	confParse(*this, fd);
}

s_conf_server::s_conf_server()
{
	this->root = DEFAULT_CONF_ROOT;
	this->ipports[DEFAULT_CONF_IP] = DEFAULT_CONF_PORT;
	this->server_names.push_back(DEFAULT_CONF_SERVNAME);
	this->client_max_body_size = DEFAULT_CONF_BODYSIZE;
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

std::string	Conf::getCurrLocation() const
{
	return (this->_currlocation);
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

void	Conf::setCurrLocation(std::string curr)
{
	this->_currlocation = curr;
}

void	Conf::updateBlock(int block_type)
{
	if (block_type == B_EVENTS)
		this->_nevents++;
	else if (block_type == B_HTTP)
		this->_nhttp++;
	else if (block_type == B_SERVER)
		this->_nserver++;
	else
		std::cerr << "\033[31mConf: Unrecognized block number\033[0m\n";
}

int		Conf::getBlockNumber(int block_type)
{
	if (block_type == B_EVENTS)
		return (this->_nevents);
	else if (block_type == B_HTTP)
		return (this->_nhttp);
	else if (block_type == B_SERVER)
		return (this->_nserver);
	std::cerr << "\033[31mConf: Unrecognized block number\033[0m\n";
	return (-1);
}

std::vector<t_conf_server>	&Conf::getConfServer(void)
{
	return (this->_srv_conf);
}

t_conf_server	&Conf::getServerBlock(void)
{
	return (this->_srvblock);
}

t_conf_location	&Conf::getLocationBlock(void)
{
	return (this->_locblock);
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

std::string	Conf::missingBlock() const
{
	if (this->_nevents > 1)
		return ("events");
	else if (this->_nhttp != 1)
		return ("http");
	else if (this->_nserver < 1)
		return ("server");
	return ("");
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

std::ostream &operator<<(std::ostream &os, Conf &c)
{
	std::cout << "\033[35mPrint of all configurations:\n";
	std::cout << "\033[33m{MAIN BLOCK}\n";
	if (c.getMainUser().empty() == false)
		std::cout << "\033[34mUser:\t\033[33m" << c.getMainUser() << "\n";
	std::cout << "\033[0m";
	return (os);
}