#include "../../hpp/Server.hpp"
#include "../../hpp/Request.hpp"

#define DIV "--------------------------------\n"

/*SECTION - gestione concorrenza

	4 strutture:				
	1)ARRAY poll: array di pollfd. Contiene tutti gli fd che POSSONO bloccare:
(enum e_fd_type)
	.FD_SERVER,
	.FD_CLIENT,
	.FD_PIPE_RD,
	.FD_PIPE_WR
				[fd srv 1]	[fd srv 2]	[fd cli 1]	[fd cli 2]	[fd pipe[0]]
enum e_fd_type:		SERVER	SERVER		CLIENT		CLIENT		FD_PIPE_RD
Client *pointer:	NULL	NULL		*self		*self <-----*
s_cgi	*pointer:	NULL	NULL		NULL		*-------->	*self
bool	cgi_ready:	false	false		false		false/true	false
*/
void Server::printPollInfo(void)
{
	std::string			types[4] = {"SERVER", "CLIENT", "PIPE_RD", "PIPE_WR"};
	Client				*client;
	s_cgi				*cgi;
	pollfd				poll_data;
	static int			connection_number;
	std::ostringstream	result;

	result << "Connection number " << connection_number++ << "\n";
	result << DIV;
	for (size_t i = this->getServerNum(); i != this->_addrs.size(); ++i)
	{
		poll_data = this->_addrs[i];
		client = this->getFdData()[poll_data.fd].client;
		cgi = this->getFdData()[poll_data.fd].cgi;
		result << "INDEX " << i << "\n";
		result << "FD " << poll_data.fd << "\n";
		result << "TYPE " << types[this->getFdData()[poll_data.fd].type] << "\n";
		if (client)
		{
			result << "Client " << client->getSockFd();
			if (client->getRequest().getUrlOriginal().empty() == false)
				result << " - " << client->getRequest().getUrlOriginal() << "\n";
			else
				result << " - ???\n";
		}
		if (cgi)
		{
			result << "pipe[0]=" << cgi->pipe[0] << ",";
			result << "pipe[1]" << cgi->pipe[1] << "\n";
		}
		result << "EVENTS: ";
		if (poll_data.events == 0)
			result << "NONE";
		if (poll_data.events & POLLIN)
			result << "POLLIN, ";
		if (poll_data.events & POLLOUT)
			result << "POLLOUT, ";
		result << "\n";
		result << "REVENTS: ";
		if (poll_data.revents == 0)
			result << "NONE";
		if (poll_data.revents & POLLIN)
			result << "POLLIN, ";
		if (poll_data.revents & POLLOUT)
			result << "POLLOUT, ";
		result << "\n";
	}
	result << DIV;
	print_file("HISTORY", result.str());
}

/**
 * @brief Prints all location configuration and parameters of a server
 * 
 * @param it 
 */
void	Server::printServerConfiguration(SrvNameMap::iterator it) const
{
	if (!SERVER)
		return ;
	std::cout << std::endl << "\033[1;37m" << "Creating server " << this->_server_num + 1<< "\033[0m" << std::endl;
	std::cout << "Listening on -> \033[1;32m" << (*it).first.first << ":" << (*it).first.second << "\033[0m" << std::endl;
	//NOTE - questa linea stampa tutto l'universo
	std::cout << (*it).second;
}
