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

*/
void	delete_method(Client &client, std::string &body, std::fstream &file)
{
	std::string	url = client.getRequest().getUrl();

	std::cout << "\033[31mMETHOD DELETE\033[0m\nbody:" << body << "\nurl:" << url << "\n";
	if (client.getRequest().getStatusCode() != 200 || \
		client.getRequest().getDnsErrorBool() == true || \
		client.getRequest().getAutoIndexBool() == true)
	{
		std::cout << "fail" << std::endl;
		if (body.empty() == false)
			body = file_opener(file);
		return ;
	}
	std::cout << "Url in delete method: " << url << std::endl;
	if (url.rbegin()[0] == '/')
		url.erase(url.find_last_of('/'), 1);
	if (std::remove(url.c_str()) == 0)
	{
		file.open("www/var/index.html");
		if (file.fail())
			return ;
		body = file_opener(file, "delete_method: cannot open file on success");
		body.replace(body.find("{MSG}"), 5, "file " + url + " deleted successfully!");
		return ;
	}
	if (valid_directory(url))
	{
		client.getRequest().setStatusCode(HTTP_CE_FORBIDDEN);
		file.open("www/var/errors/special/403_DirNotEmpty.html");
	}
	else
	{
		client.getRequest().setStatusCode(HTTP_SE_INTERNAL);
		file.open("www/var/errors/special/500_CannotDeleteFile.html");
	}
	body = file_opener(file, "delete_method: cannot open file on error");
}