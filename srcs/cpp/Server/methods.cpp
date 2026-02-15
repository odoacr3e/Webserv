#include "../../hpp/Server.hpp"
#include "../../hpp/Request.hpp"

static void execute_delete(Client &client, std::string &body, std::fstream *file);
static int	check_delete(Client &client, std::string &body, Server &srv, std::fstream *file);
// static int	ft_recv(int fd, Request &request, char *input, int bytes_first_recv);
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

void	Server::postMethod(Client &client, std::string &body, std::fstream *resp_file)
{(void)resp_file;//FIXME - togliere da prototipo
	Request	&request = client.getRequest();
	std::fstream		html;

	// return (TEST(request));
	// NOTE - trova file e fa upload in base alla location
	if (request.checkKey("Boundary") == false)
		return ;
	std::string file;
	std::string val = request.getHeader()["Content-Disposition"];
	// std::cout << "postMethod() Cont-Disp: " << val << "\n";
	if (val.find("filename=\"") != std::string::npos && val.rbegin()[0] == '"')
	{
		file = val.substr(val.find("filename=\"") + 10, val.find_last_of('\"'));
		if (file.rbegin()[0] == '\"')
			file.erase(file.length() - 1, 1);
	}
	else
		request.fail(HTTP_CE_BAD_REQUEST, "Bad \"Content-Disposition\" header format");
	if (client.getLocConf().post_storage.empty() == false)
		file = client.getLocConf().post_storage + file;
	else
		file = client.getSrvConf().post_storage + file;
	//file = url_arg_remove(client.getRequest().getUrl(), '/') + file;
	std::cout << "postMethod(): " << file << std::endl;
	if (file_checker(file))
		request.fail(HTTP_CE_CONFLICT, "File already exists!");
	std::ofstream	ofile(file.c_str(), std::ios_base::binary);
	if (ofile.fail())
		std::cout << "Error opening file\n";
	//- Aggiungiamo il numero di carattteri aggiunti dal protocollo al boundary (\r\n--boundary--\r\n)
	size_t	bound_len = request.getHeaderVal("Boundary").length() + 8;
	ofile.write(request.getBinBody().data(), request.getBinBody().size() - bound_len);
	if (request.getStatusCode() == 200)
		html.open("www/var/upload/success_upload.html");
	else
		html.open("www/var/errors/fail_upload.html");
	if (resp_file->fail())
	{
		client.getRequest().fail(HTTP_CE_NOT_FOUND, ": html not found!");
		html.open((checkErrorPages(client.getRequest())).c_str());
	}
	// std::cout << "FAILE: " << file << std::endl;
	body = file_opener(html);
	find_and_replace(body, "{MSG}", request.getFailMsg());
}

void	print_bin(std::string filename, char *bin_data, size_t len);

static void	trimBody(Request &request);

int	bodyHeaderParsing(Request &request)
{
	if (request.getBodyHeaders() == true)// body gia parsato
		return (true);
	trimBody(request);
	return (false);
}

static void	trimBody(Request &request)
{//cursore 
	size_t		h_len[2];
	size_t		bodyHeaderContentLen;
	size_t		bodyContentLen;
	std::string	temp;
	std::string	boundary;

	boundary = request.getHeaderVal("Boundary");
	h_len[0] = file_cursor_pos(request.getRequestStream());
	std::getline(request.getRequestStream(), temp, '\n');
	// std::cout << "TrimBody()\n";
	// std::cout << "temp: " << temp <<std::endl;
	// std::cout << "bound " << request.getHeaderVal("Boundary") << std::endl;
	if (temp == "")
		return ;
	else if ("--" + boundary + '\r' == temp)
	{
		headerParsing(request, false);
		//salva la posizione del cursore dopo headerParsing
		h_len[1] = file_cursor_pos(request.getRequestStream());
	}//else: è un body normale senza immagine, niente headerBodyParsing
	else
		h_len[1] = h_len[0];
	// std::cout << "TROVATO \r\n! adesso trimmo il body\n";
	bodyHeaderContentLen = request.getSockBytes() - h_len[0];
	bodyContentLen = request.getSockBytes() - h_len[1];
	request.getBytesLeft() -= bodyHeaderContentLen;
	for (size_t i = 0; i != bodyContentLen; i++)
		request.getSockBuff()[i] = request.getSockBuff()[i + h_len[1]];
	request.getBinBody().insert(request.getBinBody().end(), request.getSockBuff(), request.getSockBuff() + bodyContentLen);
	request.getBodyHeaders() = true;
}
