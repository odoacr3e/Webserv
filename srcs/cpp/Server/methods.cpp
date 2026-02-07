#include "../../hpp/Server.hpp"
#include "../../hpp/Request.hpp"

static void execute_delete(Client &client, std::string &body, std::fstream *file);
static int	check_delete(Client &client, std::string &body, Server &srv, std::fstream *file);
static int	ft_recv(int fd, Request &request, char *input, int bytes_first_recv);
int			headerParsing(Request &request, bool reset);

/*NOTE - summary

	-	DELETE
	-	POST
*/

//SECTION - DELETE

void	Server::deleteMethod(Client &client, std::string &body, std::fstream *file)
{
	if (check_delete(client, body, *this, file) != 0)
		return ;
	(*file).close();
	execute_delete(client, body, file);
}

static int	check_delete(Client &client, std::string &body, Server &srv, std::fstream *file)
{
	int					status_code;
	bool				dns;
	bool				autoindex;
	std::string			url;
	const std::string	&protected_files(srv.getProtectedFiles());

	status_code = client.getRequest().getStatusCode();
	dns = client.getRequest().getDnsErrorBool();
	autoindex = client.getRequest().getAutoIndexBool();
	url = client.getRequest().getUrl();
	if (status_code != 200 || dns == true || autoindex == true)
	{
		if (body.empty() == true)
			body = file_opener(*file);
	}
	else if (protected_files.find(url) != std::string::npos)
	{
		file->close();
		file->open("www/var/errors/403.html");
		body = file_opener(*file);
		client.getRequest().fail(HTTP_CE_FORBIDDEN);
	}
	else
		return (0);
	return (1);
}

static void execute_delete(Client &client, std::string &body, std::fstream *file)
{
	std::string url;
	
	url = client.getRequest().getUrl();
	if (url.rbegin()[0] == '/')
		url.erase(url.find_last_of('/'), 1);
	if (std::remove(url.c_str()) == 0)//file cancellato
	{
		file->open("www/var/2xx.html");
		body = file_opener(*file, "delete_method: cannot open file on success");
		find_and_replace(body, "{MSG}", "file " + url + " deleted successfully!");
		find_and_replace(body, "{CODE}", HTTP_OK_NO_CONTENT);
		return ;
	}//cancellazione fallita
	if (valid_directory(url))
	{
		client.getRequest().fail(HTTP_CE_FORBIDDEN);
		file->open("www/var/errors/special/403_DirNotEmpty.html");
	}
	else
	{
		client.getRequest().fail(HTTP_SE_INTERNAL);
		file->open("www/var/errors/special/500_CannotDeleteFile.html");
	}
	body = file_opener(*file, "delete_method: cannot open file on error");
}

//SECTION - POST

/*
	1)	decidere dove buttare roba
	2)	fare hT(ETTE)ml
*/
void	Server::postMethod(Client &client, std::string &body, std::fstream *resp_file)
{
	Request	&request = client.getRequest();

	// NOTE - trova file e fa upload in base alla location
	if (request.checkKey("Content-Type") && request.getHeader()["Content-Type"].find("multipart/form-data") != std::string::npos)
	{
		std::string file;
		std::string val = request.getHeader()["Content-Disposition"];
		// std::cout << "VAL: " << val << std::endl;
		// std::cout << request << std::endl;
		if (val.find("filename=\"") != std::string::npos && val.rbegin()[0] == '"')
		{
			file = val.substr(val.find("filename=\"") + 10, val.find_last_of('\"'));
			if (file.rbegin()[0] == '\"')//Mi vergogno, ma papà ha detto che va bene
				file.erase(file.length() - 1, 1);
		}
		else
			request.fail(HTTP_CE_BAD_REQUEST, "Bad \"Content-Disposition\" header format");
		if (!client.getLocConf().root.empty())
			file = client.getLocConf().root + file;
		else
			file = client.getSrvConf().root + file;
		if (file_checker(file))
			request.fail(HTTP_CE_CONFLICT, "File already exists!");
		std::ofstream	ofile(file.c_str(), std::ios_base::binary);
		if (ofile.fail())
			std::cout << "Error opening file\n";
		ofile.write(request.getBinBody().data(), request.getBinBody().size());
		resp_file->open("www/var/upload/index.html");
		if (resp_file->fail())
		{
			client.getRequest().fail(HTTP_CE_NOT_FOUND, ": upload file not found!");
			resp_file->open((checkErrorPages(client.getRequest())).c_str());
		}
		std::cout << "FAILE: " << file << std::endl;
		body = file_opener(*resp_file);
	}
}

int	executePost(Request &request)
{
	size_t		h_len;
	std::string	line;
	size_t		header_leftover[2];

	std::remove("REQUEST");
	print_file("REQUEST", request.getSockBuff());
	header_leftover[0] = request.getRequestStream().tellg();
	std::getline(request.getRequestStream(), line, '\n');
	headerParsing(request, false);
	header_leftover[1] = request.getRequestStream().tellg();
	request.setBodyLen(request.getBodyLen() - (header_leftover[1] - header_leftover[0]));
	h_len = request.getRequestStream().tellg();
	request.getSockBytes() -= (int)h_len;
	for (int i = 0; i != request.getSockBytes(); i++)
		request.getSockBuff()[i] = request.getSockBuff()[i + h_len];
	return (ft_recv(request.getSockFd(), request, request.getSockBuff(), request.getSockBytes()));
}

static int	ft_recv(int fd, Request &request, char *input, int bytes_first_recv)
{
	size_t				bodyLength = request.getBodyLen() - bytes_first_recv;
	int					left = bodyLength;
	char				buf[2048] = {0};
	std::vector<char>	&body = request.getBinBody();
	int 				bytes;

	if (fd < 0)
		return (-69);
	body.insert(body.begin(), input, input + bytes_first_recv);
	//SECTION - recv
	while (left)
	{
		bytes = recv(fd, buf, 2048, 0);
		if (bytes == -1)
		{
			std::cout << "Error in ft_recv by recv, left: " << left << "\n";
			break;
		}
		if (bytes == 0)
		{//FIXME - gestire se client chiude connessione mentre leggiamo
			std::cout << "connessione chiusa\n";
			break ;
		}
		if (left < 0)
		{
			std::cout << "muori JOJO! \n";
			std::cout << "abort\n";
		}
		left -= bytes;
		body.insert(body.end(), buf, buf + bytes);
	}
	return (69 - 69);
}
