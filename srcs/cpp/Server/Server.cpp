
#include "../../hpp/Server.hpp"

#define SIZE_STR "B", "KB", "MB", "GB", "TB"
#define MSG_END_CONNECTION "======================================\nchiudo connessione {INDEX}\n======================================\n"

std::string				createHtml(Client &client, const std::string &body);
struct pollfd			createServerSock(int port_n);
struct pollfd			setupPollFd(int client);
std::string				fileToString(std::string filename);
dirent					*findUrlDirectory(std::string url);

// NOTE - aggiungiamo il socket del server al vector di server
Server::Server(Conf &conf, const char **env):_env(env)
{
	pollfd	port_connection;

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

//TODO - refactoring: spostare funzioni poll, request, response file(?)
void	Server::checkForConnection() //checkare tutti i socket client per vedere se c'e stata una connessione
{
	for (std::vector<struct pollfd>::iterator it = this->_addrs.begin() + this->_server_num; it != this->_addrs.end(); ++it)
	{
		if ((*it).fd != -1 && ((*it).revents & POLLIN)) // revents & POLLIN -> pronto per leggere
		{
			char buffer[2048] = {0};
			// std::cout << "bytes left: " << this->_clients[(*it).fd]->getRequest().getBytesLeft() << "\n";
			int bytes = recv((*it).fd, buffer, sizeof(buffer) - 1, 0);
			print_file("REQUEST", buffer, bytes);
			if (bytes <= 0)
			{
				static int	n;
				//da mettere in una funzione a parte
				std::string msgEndCon(MSG_END_CONNECTION);
				find_and_replace(msgEndCon, "{INDEX}", n++);
				std::cout << msgEndCon << "\033[2J\033[H";
				print_file("REQUEST", msgEndCon);
				close((*it).fd);
				if (this->_clients[(*it).fd])
				{
					delete this->_clients[(*it).fd];
					this->_clients.erase((*it).fd);
					it = this->_addrs.erase(it) - 1;
				}
			}
			else
				processRequest(it, buffer, bytes);
		}
		else if ((*it).fd != -1 && ((*it).revents & POLLOUT)) // revents & POLLOUT -> pronto per ricevere
		{
			std::string	html = createResponse(*(this->_clients[(*it).fd]));
			send((*it).fd, html.c_str(), html.length(), 0);
			static int	n_resp;
			print_file("RESPONSE", html);
			std::string msgEndCon(MSG_END_CONNECTION);
			find_and_replace(msgEndCon, "{INDEX}", n_resp++);
			print_file("RESPONSE", msgEndCon);
			std::vector<char>	&contentData = this->_clients[(*it).fd]->getBuffer();
			if (this->_clients[(*it).fd]->sendContentBool() == true)
				send((*it).fd, contentData.data(), contentData.size(), 0);
			this->_clients[(*it).fd]->sendContentBool() = false;
			(*it).events = POLLIN;
		}
	}
}

void	Server::printServerConfiguration(SrvNameMap::iterator it) const
{
	if (!SERVER)
		return ;
	std::cout << std::endl << "\033[1;37m" << "Creating server " << this->_server_num + 1<< "\033[0m" << std::endl;
	std::cout << "Listening on -> \033[1;32m" << (*it).first.first << ":" << (*it).first.second << "\033[0m" << std::endl;
	//NOTE - questa linea stampa tutto l'universo
	//std::cout << (*it).second;
}



/* // NOTE - crea un socket listen per il server che vogliamo creare
struct pollfd	createServerSock(int port_n)
{
	struct sockaddr_in	address;
	int					server_fd;
	struct pollfd		srv;
	int					opt = 1;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		throw std::runtime_error("\033[31mSocket ha fallito.\033[0m");
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;// 7F000001 
	address.sin_port = htons(port_n);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) != 0)
	{
		close(server_fd);
		return (CONNECTION_FAIL);
	}
	if (listen(server_fd, MAX_CONNECTION) != 0)
	{
		close(server_fd);
		throw std::runtime_error("\033[31mIl server ha le orecchie tappate.\033[0m");
	}
	srv.fd = server_fd;
	srv.events = POLLIN;
	srv.revents = 0;
	return (srv);
} */

/* std::string	fileToString(std::string filename)
{
	std::ifstream	fd(filename.c_str());
	std::string		file;

	if (fd.fail())
		abort();
	std::getline(fd, file, '\0');
	return (file);
} */

// NOTE - creiamo oggetto client e lo aggiungiano alla mappa di puntatori client 
void	Server::addSocket(int index)
{
	int client = accept(this->_addrs.data()[index].fd, NULL, NULL);
	if (client == -1)
		throw std::runtime_error("\033[31mconnessione non accettata.\n\033[0m");
	this->_addrs.push_back(setupPollFd(client));
	this->_clients[client] = new Client(client, this->_addrs.data()[index].fd);
}
