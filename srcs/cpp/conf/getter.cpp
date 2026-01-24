#include "../../hpp/Conf.hpp"

/*NOTE - index of content
		-	block type
		-	main block
		-	server block
		-	location block
		-	ip port
		-	SrvNameMap
		-	errPages
*/

//SECTION - block type

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

int		Conf::getBlockNumber(int block_type) const
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

int	Conf::getBlockType(void) const
{
	if (this->_location)
		return (B_LOCATION);
	else if (this->_server)
		return (B_SERVER);
	else if (this->_http)
		return (B_HTTP);
	else if (this->_events)
		return (B_EVENTS);
	return (B_NONE);
}

//SECTION - main block

// main block
std::string	Conf::getMainUser(void) const
{
	return (this->_user);
}

//SECTION - server block

std::vector<t_conf_server>	&Conf::getConfServer(void)
{
	return (this->_srv_conf);
}

t_conf_server	&Conf::getServerBlock(void)
{
	return (this->_srvblock);
}

//SECTION - location block

t_conf_location	&Conf::getLocationBlock(void)
{
	return (this->_locblock);
}

t_conf_location	Conf::getCopyLocationBlock(void) const
{
	return (this->_locblock);
}

std::string	Conf::getCurrLocation() const
{
	return (this->_locblock.path);
}

std::string t_conf_location::getRetUri() const
{
	return (this->ret_uri);
}

int t_conf_location::getRetCode() const
{
	return (this->ret_code);
}

//SECTION - ip port

//returns the number of IpPort.
int	Conf::getIpPortNumber() const
{
	return (this->_nipport);
}

//returns the pair ipport in position i.
std::pair<std::string, int>	&Conf::getPairIpPort(int i)
{
	return (this->_ipport[i]);
}

//returns the pair ipport vector, as a reference.
std::vector<std::pair<std::string, int> >	&Conf::getIpPort(void)
{
	return (this->_ipport);
}

//SECTION - SrvNameMap

SrvNameMap	&Conf::getSrvNameMap()
{
	return (this->_srvnamemap);
}
