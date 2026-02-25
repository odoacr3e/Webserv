
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

	std::memset(&fd_data, 0, sizeof(s_fd_data));
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
	s_cgi	*temp;
	int		pipes[2];

	//SECTION - client disconnection
	for (std::vector<struct pollfd>::iterator it = this->_addrs.begin() + this->_server_num; it != this->_addrs.end(); ++it)
	{
		if (this->_fd_data[it->fd].type == FD_PIPE_RD || \
		this->_fd_data[it->fd].type == FD_PIPE_WR)
		{
			temp = this->_fd_data[it->fd].cgi;
			if (temp != NULL)
			{
				delete this->_fd_data[it->fd].cgi;
				pipes[0] = this->_fd_data[it->fd].cgi->pipe[0];
				pipes[1] = this->_fd_data[it->fd].cgi->pipe[1];
				if (this->_fd_data[it->fd].type == FD_PIPE_RD && pipes[1] != 0)
					this->_fd_data[pipes[1]].cgi = NULL;
				else if (this->_fd_data[it->fd].type == FD_PIPE_WR && pipes[0] != 0)
					this->_fd_data[pipes[0]].cgi = NULL;
			}
			this->_fd_data[it->fd].cgi = NULL;
		}
		close((*it).fd);
		delete this->_clients[(*it).fd];
		this->_clients.erase((*it).fd);
		it = this->_addrs.erase(it) - 1;
	}
	//SECTION - server disconnection
	for (int i = 0; i != this->_server_num; i++)
		close(this->_addrs[i].fd);
	//SECTION - socket buffer clearup

}

void	Server::checkForConnection() //checkare tutti i socket client per vedere se c'e stata una connessione
{
	pollfd	poll_data;
	Client	*client;
	s_cgi	*cgi;

	this->printPollInfo();
	for (size_t i = this->_server_num; i != this->_addrs.size(); ++i)
	{
		poll_data = this->_addrs[i];
		client = this->getFdData()[poll_data.fd].client;
		cgi = this->getFdData()[poll_data.fd].cgi;
		if (poll_data.revents & POLLIN) // revents & POLLIN -> pronto per leggere
		{
			if (this->getFdData()[poll_data.fd].type == FD_PIPE_RD)
				client->readCgi(*this, *cgi), i--;
			else
			{
				char buffer[2048] = {0};//NOTE - reserve vector
				int bytes = recv(poll_data.fd, buffer, sizeof(buffer) - 1, 0);
				if (bytes <= 0)
					eraseClient(*client, i--);
				else
					processRequest(*client, buffer, bytes);
			}
		}
		else if (poll_data.revents & POLLOUT) // revents & POLLOUT -> pronto per ricevere
		{
			if (this->getFdData()[poll_data.fd].type == FD_PIPE_WR)
				client->writeCgi(*this, *cgi), i--;
			else
				processResponse(*client);
		}
	}
}

/**
 * @brief Erases the connection of client after the request reading is done
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
	if ((size_t)i != this->_addrs.size() - 1)
	{
		fd = this->_addrs.back().fd;
		std::swap(this->_addrs.back(), this->_addrs[i]);
		if (this->_clients[fd])
			this->_clients[fd]->setPollFd(&this->_addrs[i]);
	}
	this->_addrs.pop_back();
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
