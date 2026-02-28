#include "../../hpp/Server.hpp"


struct pollfd *Server::getAddrs(void)
{
	return (this->_addrs.data());
}

std::vector<pollfd>	&Server::getAddrsVector(void)
{
	return (this->_addrs);
}

size_t	Server::getAddrSize(void) const
{
	return (this->_addrs.size());
}

int	Server::getServerNum() const
{
	return (this->_server_num);
}

SrvNameMap		&Server::getSrvNameMap() const
{
	return (*this->_srvnamemap);
}

ipPortCgiMap	&Server::getIpPortCgiMap()
{
	return (this->_ipPortCgiPair);
}

const std::string	&Server::getProtectedFiles() const
{
	return (this->_protected_files);
}

std::vector<struct s_fd_data>	&Server::getFdData()
{
	return (this->_fd_data);
}
