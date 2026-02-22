#include "../../hpp/Server.hpp"
#include "../../hpp/Request.hpp"

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
	std::string	types[4] = {"SERVER", "CLIENT", "PIPE_RD", "PIPE_WR"};
	Client	*client;
	s_cgi	*cgi;
	pollfd	poll_data;

	std::cout << "print_info(): \n";
	for (size_t i = 0; i != this->_addrs.size(); ++i)
	{
		poll_data = this->_addrs[i];
		client = this->getFdData()[poll_data.fd].client;
		cgi = this->getFdData()[poll_data.fd].cgi;
		std::cout << "\033[32mINDEX \033[0m" << i << "\n";
		std::cout << "\033[33mFD \033[0m" << poll_data.fd << "\n";
		std::cout << "\033[33mTYPE \033[0m" << types[this->getFdData()[poll_data.fd].type] << "\n";
		if (client)
		{
			std::cout << "\033[34mClient \033[0m" << client->getSockFd();
			if (client->getRequest().getUrlOriginal().empty() == false)
				std::cout << " - " << client->getRequest().getUrlOriginal() << "\n";
			else
				std::cout << "\n";
		}
		if (cgi)
		{
			std::cout << "\033[34mpipe[0]=\033[0m" << cgi->pipe[0] << ",";
			std::cout << "\033[34mpipe[1]=\033[0m" << cgi->pipe[1] << "\n";
		}
		std::cout << "\033[33mEVENTS\033[0m: ";
		if (poll_data.events == 0)
			std::cout << "NONE, ";
		if (poll_data.events & POLLIN)
			std::cout << "\033[" << (31 + ((poll_data.revents & POLLIN) != 0)) << "mPOLLIN, ";
		if (poll_data.events & POLLOUT)
			std::cout << "\033[" << (31 + ((poll_data.revents & POLLOUT) != 0)) << "mPOLLOUT, ";
		std::cout << "\b\b \033[0m\n";
	}
	std::cout << "END\n";
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
