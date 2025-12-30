#include "../../hpp/Conf.hpp"

/*NOTE - index of content
		-	block type
		-	main block
		-	location block
		-	ip port
		-	SrvNameMap
		-	Server name
*/

//SECTION - block type

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

//increase by one the counter of the specified block type.
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

//SECTION - main block

void		Conf::setMainUser(std::string user)
{
	this->_user = user;
}

//SECTION - location block

void	Conf::setLocation(bool val)
{
	this->_location = val;
}

void	Conf::setCurrLocation(std::string curr)
{
	this->_locblock.path = curr;
}

//SECTION - ip port

void	Conf::incrementIpPortNumber(void)
{
	this->_nipport += 1;
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

//SECTION - Server Name

void		Conf::addServerName(std::string name)
{
	this->_server_names[name] = name;
	this->_srvblock.server_names.push_back(name);
}
