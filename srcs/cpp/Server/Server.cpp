
#include "../../hpp/Server.hpp"

std::string				createHtml(Client &client, const std::string &body);
struct pollfd			createServerSock(int port_n);
struct pollfd			setupPollFd(int client);
std::string				fileToString(std::string filename);
dirent					*findUrlDirectory(std::string url);

// NOTE - aggiungiamo il socket del server al vector di server
Server::Server(Conf &conf, const char **env):_env(env)
{
	pollfd		port_connection;
	s_fd_data	fd_data;

	fd_data.type = FD_SERVER;
	this->_fd_data.resize(MAX_CONNECTION + 1);
	this->_server_num = 0;
	if (conf.getSrvNameMap().size() == 0)
		throw (std::runtime_error("\nSpecify at least one listen in conf file"));
	this->_srvnamemap = &conf.getSrvNameMap();
	for (SrvNameMap::iterator it = conf.getSrvNameMap().begin(); \
		it != conf.getSrvNameMap().end(); it++)
	{
		port_connection = createServerSock((*it).first.second);
		if (port_connection.fd != -1)
		{
			this->_server_data[port_connection.fd] = &(*it).second;
			this->_addrs.push_back(port_connection);
			printServerConfiguration(it);
			std::memset(&fd_data, 0, sizeof(s_fd_data));
			this->_fd_data[port_connection.fd] = fd_data;
			this->_server_num++;
		}
		else
			std::cout << std::endl << "\033[1;31mCan't bind ip:port -> " << (*it).first.first << ":" << (*it).first.second << std::endl;
	}
	if (this->_server_num == 0)
		throw (std::runtime_error("\nNo server could be binded."));
	ft_ls("./", this->_protected_files);
}

Server::~Server()
{
	std::cout << "\033[32mserver destructor!\033[0m" << std::endl;
	this->suppressSocket();
}

void Server::suppressSocket()
{
	//SECTION - client disconnection
	for (std::vector<struct pollfd>::iterator it = this->_addrs.begin() + this->_server_num; it != this->_addrs.end(); ++it)
	{
		close((*it).fd);
		delete this->_clients[(*it).fd];
		this->_clients.erase((*it).fd);
		it = this->_addrs.erase(it) - 1;
	}
	//SECTION - server disconnection
	for (int i = 0; i != this->_server_num; i++)
		close(this->_addrs[i].fd);
	//SECTION - socket buffer clearup
	for (packetBuffer::iterator it = this->_packet_buffer.begin(); \
	it != this->_packet_buffer.end(); it++)
		delete [] (*it);
}

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
void Server::print_info(void)
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
			std::cout << "\033[34mClient \033[0m" << client->getSockFd() << "\n";
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

void	Server::checkForConnection() //checkare tutti i socket client per vedere se c'e stata una connessione
{
	Client	*client;
	pollfd	poll_data;

	this->print_info();
	for (size_t i = this->_server_num; i != this->_addrs.size(); ++i)
	{
		poll_data = this->_addrs[i];
		client = this->getFdData()[poll_data.fd].client;
		if (poll_data.revents & POLLIN) // revents & POLLIN -> pronto per leggere
		{
			if (this->getFdData()[poll_data.fd].type == FD_PIPE_RD)
			{
				std::cout << WHITE "before readToCgi() \n\033[0m";
				this->print_info();
				client->readToCgi(*this, *this->getFdData()[poll_data.fd].cgi);
				std::cout << WHITE "after readToCgi() \n\033[0m";
				this->print_info();
				i--;
				continue ;
			}
			char buffer[2048] = {0};
			int bytes = recv(poll_data.fd, buffer, sizeof(buffer) - 1, 0);
			if (bytes <= 0)
				eraseClient(*client, i--);
			else
				processRequest(*client, buffer, bytes);
		}
		else if (poll_data.revents & POLLOUT) // revents & POLLOUT -> pronto per ricevere
		{
			if (this->getFdData()[poll_data.fd].type == FD_PIPE_WR)
			{
				client->writeToCgi(*this, *this->getFdData()[poll_data.fd].cgi);
				i--;
				continue ;
			}
			processResponse(*client);
		}
	}
}

/**
 * @brief Erases the connection of *it client after the request reading is done
 * 
 * @param it Client iterator 
*/
void		Server::eraseClient(Client &client, int i)
{
	static int	n;
	int			fd;
	std::string msgEndCon(MSG_END_CONNECTION);

	find_and_replace(msgEndCon, "{INDEX}", n++);
	std::cout << msgEndCon << "\033[2J\033[H";
	print_file("REQUEST", msgEndCon);
	fd = client.getSockFd();
	if (this->_clients[fd])
	{
		delete this->_clients[fd];
		this->_clients.erase(fd);
	}
	close_fd(&fd);
	std::swap(this->_addrs.back(), this->_addrs[i]);
	this->_addrs.pop_back();
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


/// @brief creiamo un socket, lo settiamo in base al tipo, lo aggiungiamo a poll
/// @param index fd del server in ascolto/della pipe
/// @param type FD_CLIENT/ FD_PIPE_RD / FD_PIPE_WR
/// @return position in the poll array
int	Server::addSocket(int index, e_fd_type type)
{
	pollfd		polldata;
	s_fd_data	fd_data;
	int 		socket;

	std::memset(&fd_data, 0, sizeof(s_fd_data));
	fd_data.type = type;
	fd_data.cgi_ready = false;
	if (type == FD_CLIENT)
		socket = accept(this->_addrs.data()[index].fd, NULL, NULL);
	else
		socket = index;
	if (socket == -1)
		throw std::runtime_error("\033[31mconnessione non accettata.\n\033[0m");
	polldata = setupPollFd(socket);
	this->_addrs.push_back(polldata);
	this->_fd_data[polldata.fd] = fd_data;
	if (type == FD_CLIENT)
	{
		this->_clients[socket] = new Client(socket, this->_addrs.data()[index].fd);
		(*this->_clients[socket]).setPollFd(&this->_addrs[this->_addrs.size() - 1]);
		this->_fd_data[polldata.fd].client = this->_clients[socket];
	}
	else if (type == FD_PIPE_WR)
		this->_addrs.back().events = POLLOUT;
	return (this->_addrs.size() - 1);
}
