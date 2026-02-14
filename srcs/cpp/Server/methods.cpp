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

/*
	1)	decidere dove buttare roba
	2)	fare hT(ETTE)ml
*/

//curl -X POST http://localhost:9021/upload/files/ -F "file=@PIPPO.ico"
// /upload/files/
// /www/var/upload/files/ + filename
// PIPPO.ico

void	Server::postMethod(Client &client, std::string &body, std::fstream *resp_file)
{
	Request	&request = client.getRequest();

	// return (TEST(request));
	// NOTE - trova file e fa upload in base alla location
	if (request.checkKey("Content-Type") && request.getHeader()["Content-Type"].find("multipart/form-data") != std::string::npos)
	{
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
		file = url_arg_remove(client.getRequest().getUrl(), '/') + file;
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
			resp_file->open("www/var/upload/success_upload.html");
		else
			resp_file->open("www/var/errors/fail_upload.html");
		if (resp_file->fail())
		{
			client.getRequest().fail(HTTP_CE_NOT_FOUND, ": upload file not found!");
			resp_file->open((checkErrorPages(client.getRequest())).c_str());
		}
		// std::cout << "FAILE: " << file << std::endl;
		body = file_opener(*resp_file);
		find_and_replace(body, "{MSG}", request.getFailMsg());
	}
}

// POST /srcs/ pippo
// : www/var/Files/pippo
// GET /srcs/ pippo
// : www/var/pippo
// : www/var/Files/pippo

void	print_bin(std::string filename, char *bin_data, size_t len);

/*
POST /upload/ HTTP/1.1
Host: localhost:9021
Connection: keep-alive
Cache-Control: max-age=0
sec-ch-ua: "Not(A:Brand";v="8", "Chromium";v="144", "Brave";v="144"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Linux"
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/144.0.0.0 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,\\**;q=0.8
Sec-GPC: 1
Accept-Language: en-US,en;q=0.5
Sec-Fetch-Site: cross-site
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Accept-Encoding: gzip, deflate, br, zstd

header nome file
header pippo

header gabibbo
*/
bool	trimBody(Request &request);

int	bodyHeaderParsing(Request &request)
{
	if (request.getBodyHeaders() == true) // body gia parsato
		return (true);
	return (trimBody(request));
	//request.getBodyHeaders() = trimBody(request);
	//return (request.getBodyHeaders());
}

bool	trimBody(Request &request)//se finisce di leggere torna true
{
	size_t		h_len;
	std::string	temp;
	std::string	boundary;

	//salva la posizione del cursore dopo headerParsing
	boundary = request.getHeaderVal("Boundary");
	h_len = file_cursor_pos(request.getRequestStream());
	std::getline(request.getRequestStream(), temp, '\n');
	// std::cout << "TrimBody()\n";
	// std::cout << "temp: " << temp <<std::endl;
	// std::cout << "bound " << request.getHeaderVal("Boundary") << std::endl;
	if (temp == "")
		return (false);
	else if ("--" + boundary + '\r' == temp)
	{
		// std::cout << "trimBody " << request.getHeaderVal("Content-Type") << std::endl;
		if (headerParsing(request, false) != 0)
			{;}//dare 400: errore bodyHeaderParsing
		//salva la posizione del cursore dopo headerParsing
		h_len = file_cursor_pos(request.getRequestStream()) - h_len;
		request.getBytesLeft() -= request.getSockBytes();
	}//else: è un body normale senza immagine, niente headerBodyParsing
	else
		request.getBytesLeft() -= (request.getSockBytes() - h_len);
	// std::cout << "TROVATO \r\n! adesso trimmo il body\n";
	request.getSockBytes() -= (int)h_len;
	for (int i = 0; i != request.getSockBytes(); i++)
		request.getSockBuff()[i] = request.getSockBuff()[i + h_len];
	request.getSockBytes() += (int)h_len;
	request.getBinBody().insert(request.getBinBody().end(), request.getSockBuff(), request.getSockBuff() + request.getSockBytes() - (int)h_len);
	
	//request.getBytesLeft() -= request.getSockBytes();
	request.getBodyHeaders() = true;
	return (false);
}
