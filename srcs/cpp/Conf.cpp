
#include "../hpp/Conf.hpp"

Conf::Conf(std::string filepath): _file(filepath)
{
	std::ifstream fd(filepath.c_str(), std::ios_base::in);

	if (fd.fail())
		throw ConfException("Invalid configuration file");
	this->_events = 0;
	this->_http = 0;
	this->_server = 0;
	this->_location = 0;
	this->_nevents = 0;
	this->_nhttp = 0;
	this->_nserver = 0;
	this->_nipport = 0;
	this->_srvblock.client_max_body_size = 0;
	confParse(*this, fd);
}

void	s_conf_server::set_if_empty(Conf &conf)
{
	this->location["/"] = conf.getCopyLocationBlock();
	if (this->root.empty())
		this->root = DEFAULT_CONF_ROOT;
	if (*this->root.rbegin() != '/')
		this->root.push_back('/');
	if (!this->index.empty() && !valid_file(this->root + this->index))
		throw (Conf::ConfException("cannot open " + this->root + this->index + "\n"));
	if (this->server_names.size() == 0)
		this->server_names.push_back(DEFAULT_CONF_SERVNAME);
	if (this->client_max_body_size == 0)
		this->client_max_body_size = DEFAULT_CONF_BODYSIZE;
	if (this->listen_set == false)
		conf.setIpPort(std::string(DEFAULT_CONF_IP), DEFAULT_CONF_PORT);
	for (int i = conf.getIpPortNumber(); i < (int)conf.getIpPort().size(); i++)
	{
		conf.getSrvNameMap()[conf.getPairIpPort(i)] = *this;
		conf.incrementIpPortNumber();
	}
}

void	s_conf_location::set_if_empty(Conf &conf)
{
	(void)conf;
	if (this->path.empty())
		this->path = "/";
	if (this->root.empty())
		this->root = "";
	if (this->alias.empty())
		this->alias = "";
	if (this->proxy_pass.empty())
		this->proxy_pass = "";
}

void	s_conf_server::set(void)
{
	this->root.clear();
	this->index.clear();
	this->server_names.clear();
	this->client_max_body_size = 0;
	this->location.clear();
	this->listen_set = false;
}

void	s_conf_location::set(std::string path)
{
	this->path = path;
	this->root.clear();
	this->alias.clear();
	this->proxy_pass.clear();
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

int	Conf::getIpPortNumber() const
{
	return (this->_nipport);
}

std::string	Conf::getCurrLocation() const
{
	return (this->_locblock.path);
}

SrvNameMap	&Conf::getSrvNameMap()
{
	return (this->_srvnamemap);
}

/*
void	Conf::setSrvNameMap(SrvNameMap curr)
{
	std::string	error;

	for (SrvNameMap::iterator it = curr.begin(); it != curr.end(); ++it)
	{
		if (this->_srvnamemap.count((*it).first) != 0)
		{
			error = "Duplicated ip, port server name \
			combination: ";
			error += (*it).first.first + ":" + ft_to_string((*it).first.second);
			throw Conf::ConfException(error);
		}
		this->_srvnamemap[(*it).first] = this->_srvblock.server_names;
	}
}*/

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
	this->_locblock.path = curr;
}

void	Conf::setIpPort(std::string ip, int port)
{
	if (checkIpPort(ip, port) != 0)
	{
		std::string	err = "ip:port -> " + ip + ":";
		err += port;
		err += " already used";
		throw (Conf::ConfException(err));
	}
	this->_ipport.push_back(std::pair<std::string, int>(ip, port));
}

std::pair<std::string, int>	&Conf::getPairIpPort(int i)
{
	return (this->_ipport[i]);
}

std::vector<std::pair<std::string, int> >	&Conf::getIpPort(void)
{
	return (this->_ipport);
}

void	Conf::incrementIpPortNumber(void)
{
	this->_nipport += 1;
}

bool	Conf::checkIpPort(std::string ip, int port) const
{
	std::pair<std::string, int>	ipport(ip, port);

	//FIXME - si può usare <algorithm> ???
	if (std::find(this->_ipport.begin(), this->_ipport.end(), ipport) != this->_ipport.end())
		return (1);
	return (this->_srvnamemap.count(ipport) >= 1);
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

t_conf_location	Conf::getCopyLocationBlock(void)
{
	return (this->_locblock);
}

std::string	Conf::checkOpenBlock(void) const
{
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
	// this->_srvnamemap.count(std::pair<>)
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
		os << c.getConfServer()[i];
	}
	os << c.getSrvNameMap();
	os << "\033[0m";
	return (os);
}

std::ostream &operator<<(std::ostream &os, t_conf_server &srv)
{
	os << "\033[35mPrinting server " << srv.server_names[0];
	os << "\n\033[34mServer names:\033[33m";
	for (size_t i = 0; i < srv.server_names.size(); i++)//per ogni nome
		os << "\n" << srv.server_names[i];
	os << "\n";
	print_map(os, srv.location, "location", "\033[34m");
	os << "\n\033[34mip ports:\033[33m";
	os << "\n\033[34mroot: \033[33m" << srv.root;
	os << "\n\033[34mindex: \033[33m" << srv.index;
	os << "\n\033[34mbody_size: \033[33m" << srv.client_max_body_size << std::endl;
	return (os);
}

std::ostream &operator<<(std::ostream &os, t_conf_location &loc)
{
	os << "\n\033[35mPrinting location " << loc.path;
	os << "\n\033[34malias: \033[33m" << loc.alias;
	os << "\n\033[34mproxy_pass: \033[33m" << loc.proxy_pass;
	os << "\n\033[34mroot: \033[33m" << loc.root << std::endl;
	return (os);
}

std::ostream &operator<<(std::ostream &os, SrvNameMap &map)
{
	(void)map;
	os << std::endl << "\n\033[34mIp Addresses list: \033[0m" << std::endl;
	//print_map(map, "serverNameMap", "\033[34m");
	/*
		for (SrvNameMap::iterator it = c.getSrvNameMap().begin(); it != c.getSrvNameMap().end(); ++it)
	{
		os << "IP ADDRESS -> " << (*it).first.first << ":" << (*it).first.second << \
		" | SERVER NAME -> ";
		for (size_t i = 0; i < (*it).second.size(); i++)
		{
			os << (*it).second[i];
			if (i + 1 != (*it).second.size())
				os << ", ";
			else
				os << std::endl;
		}*/
	return (os);
}
