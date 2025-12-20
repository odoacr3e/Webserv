
#include "../hpp/Conf.hpp"

Conf::Conf(std::string filepath): _file(filepath)
{
	std::ifstream fd(filepath.c_str(), std::ios_base::in);

//	std::memset((void*)this, 0, sizeof(this));//NOTE - se scommenti linea sotto crasha std::map
//	std::memset((void*)&this->_srvblock, 0, sizeof(this->_srvblock));
	this->_events = 0;
	this->_http = 0;
	this->_server = 0;
	this->_location = 0;
	this->_nevents = 0;
	this->_nhttp = 0;
	this->_nserver = 0;
	this->_srvblock.client_max_body_size = 0;
//	std::memset((void*)&this->_locblock, 0, sizeof(this->_locblock));
	if (fd.fail())
		throw ConfException("Invalid configuration file");
	confParse(*this, fd);
}

void	s_conf_server::set_if_empty(void)
{
	if (this->root.empty())
		this->root = DEFAULT_CONF_ROOT;
	if (this->ipports.size() == 0)
		this->ipports[DEFAULT_CONF_IP] = DEFAULT_CONF_PORT;
	if (this->server_names.size() == 0)
		this->server_names.push_back(DEFAULT_CONF_SERVNAME);
	if (this->client_max_body_size == 0)
		this->client_max_body_size = DEFAULT_CONF_BODYSIZE;
}

void	s_conf_server::set(void)
{
	this->root.clear();
	this->ipports.clear();
	this->server_names.clear();
	this->client_max_body_size = 0;
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
	if (val == true)
		this->_srvblock.set();
	this->_server = val;
}

void	Conf::setLocation(bool val)
{
	this->_location = val;
}

void	Conf::setCurrLocation(std::string curr)
{
	this->_locblock.path = curr;
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

void		Conf::addServerName(std::string name)
{
	this->_server_names[name] = name;
	this->_srvblock.server_names.push_back(name);
}

bool		Conf::findServerName(std::string name)
{
	return (this->_server_names.count(name) > 0);
}

std::ostream &operator<<(std::ostream &os, Conf &c)
{
	os << "####################################\n";
	os << "\033[35mPrint of all configurations:\n";
	os << "\033[33m{MAIN BLOCK}\n";
	if (c.getMainUser().empty() == false)
		os << "\033[34mUser:\t\033[33m" << c.getMainUser() << "\n";
	os << "\033[33m{SERVER BLOCK}";
	for (size_t i = 0; i < c.getConfServer().size(); i++)//per ogni server
	{
		os << "\n\033[35mPrinting " << c.getConfServer()[i].server_names[0];
		os << "\n\033[34mServer names:\033[33m";
		for (size_t j = 0; j < c.getConfServer()[i].server_names.size(); j++)//per ogni nome
			os << "\n" << c.getConfServer()[i].server_names[j];
		os << "\n\033[34mip ports:\033[33m";
		for (std::map<std::string, int>::iterator j = c.getConfServer()[i].ipports.begin(); \
		j != c.getConfServer()[i].ipports.end(); j++)
			os << "\nlistening on " << (*j).first << ":" << (*j).second;
		os << "\n\033[34mroot: \033[33m" << c.getConfServer()[i].root;
		os << "\n\033[34mbody_size: \033[33m" << c.getConfServer()[i].client_max_body_size;
	}
	os << "\033[0m";
	return (os);
}