#include "../../hpp/Server.hpp"

dirent	*findUrlDirectory(std::string url);

//NOTE - crea una struct pollfd con l'fd del client, dato da accept()
struct pollfd	setupPollFd(int client)
{
	struct pollfd s;

	s.fd = client;
	s.events = POLLIN;
	s.revents = 0;
	return (s);
}

// NOTE - crea un socket listen per il server che vogliamo creare
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
}

std::string	fileToString(std::string filename)
{
	std::ifstream	fd(filename.c_str());
	std::string		file;

	if (fd.fail())
		abort();
	std::getline(fd, file, '\0');
	return (file);
}

/*
	1) se errore o autoindex ---> 	leggi pagina di errore ed esci.
	2) se file non esiste ------>	gia gestito prima
	3) se file non ha per.
	messi di cancellaz. ----->	401 not allowed OPPURE 403 forbidden	

	4) se directory ------>	cancella tutti file da directory ---|--> SPREAD DEMOCRACY
							OPPURE dai sempre 403 forbidden		|
	
	5) se file ----------->	LIBERATION
*/
void	delete_method(Client &client, std::string &body, std::fstream &file)
{
	std::string	url = client.getRequest().getUrl();

	std::cout << "\033[31mMETHOD DELETE\033[0m\nbody:" << body << "\nurl:" << url << "\n";
	//1)	errore/autoindex
	if (client.getRequest().getStatusCode() != 200 || \
	client.getRequest().getRequestErrorBool() == true || \
	client.getRequest().getAutoIndexBool() == true)
	{
		if (body.empty() == false)
			body = file_opener(file);
		return ;
	}
	//2)	file esiste, gia controllato prima
	//3)	bisogna controllare si ho mettiamo tutto in un array con va bene ti stavo ascoltanto davero con una v smetti di parlare da solo GOLLUM
	if (valid_directory(url) == true)//4)	se directory
	{
		dirent	*file = findUrlDirectory(url);
		while (file)
		{
			if (file->d_name[0] != '.')
				std::remove(file->d_name);
			file = findUrlDirectory(url);
		}
	}
	else//5)	se file
		std::remove(url.c_str());
}