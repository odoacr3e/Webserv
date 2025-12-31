
#include "../hpp/Conf.hpp"

/*NOTE - index of content
		-	Conf class
		-	s_conf_server
		-	s_conf_location
		-	ip port check
		-	block checks
		-	server name checks
*/
//SECTION - Conf class

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

//SECTION - s_conf_server

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

void	s_conf_server::set(void)
{
	this->root.clear();
	this->index.clear();
	this->server_names.clear();
	this->client_max_body_size = 0;
	this->location.clear();
	this->listen_set = false;
}

//SECTION - s_conf_location

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

void	s_conf_location::set(std::string path)
{
	this->path = path;
	this->root.clear();
	this->alias.clear();
	this->proxy_pass.clear();
}

//SECTION - ip port check

bool	Conf::checkIpPort(std::string ip, int port) const
{
	std::pair<std::string, int>	ipport(ip, port);

	//FIXME - si può usare <algorithm> ???
	if (std::find(this->_ipport.begin(), this->_ipport.end(), ipport) != this->_ipport.end())
		return (1);
	return (this->_srvnamemap.count(ipport) >= 1);
}

//SECTION - block checks

//NOTE - returns current block name, or ""
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

//NOTE - returns missing block name, or ""
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

//returns enum code if type is valid.
//else, returns B_NONE (== 0).
//in case of error, returns an enum e_conf_error. 
int	Conf::checkBlockType(std::string &type) const
{
	if ((type == "events" && _nevents > 0) || \
	(type == "http" && _nhttp > 0))
		return (CONF_MULT_BLOCK);
	if (type == "events" && !_http && !_events)
		return (B_EVENTS);
	else if (type == "http" && !_http && !_events)
		return (B_HTTP);
	else if (type == "server" && _http && !_server)
		return (B_SERVER);
	else if (type == "location" && _server)
		return (B_LOCATION);
	if (type == "events" || type == "http" || \
	type == "server" || type == "location")
		return (CONF_BLOCK_OPEN);	
	return (B_NONE);
}

//SECTION - server name checks

bool		Conf::findServerName(std::string name)
{
	return (this->_server_names.count(name) > 0);
}
