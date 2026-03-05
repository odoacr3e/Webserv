#include "../../hpp/Server.hpp"
#include "../../hpp/Request.hpp"

static void execute_delete(Client &client, std::string &body, std::fstream *file);
static int	check_delete(Client &client, std::string &body, Server &srv, std::fstream *file);
int			headerParsing(Request &request, bool reset);
void		fill_error_page(Client &client, std::string &html);
std::string	get_filename(Client &client);
void		write_on_ofile(Request &request, std::string file);
void		choose_html(Server &srv, Client &client, std::fstream &file, std::fstream &html);

/**
 * @brief executes on of three valid methods POST GET DELETE
 * 
 * @param client > Client containing request
 */
void	Server::runMethod(Client &client)
{
	if (this->resp_body.empty() == false)
		return ;
	if (client.getRequest().getFailMsg().empty() == false)
	{
		this->resp_body = open_and_read(this->file, "runMethod GET: Cannot open file");
		fill_error_page(client, this->resp_body);
		return ;
	}
	if (client.getRequest().getRunScriptBool() == true)
		run_script(*this, client, this->resp_body);
	switch (client.getRequest().getMethodEnum())
	{
		case GET:
			this->getMethod(client);
			break ;
		case DELETE:
			this->deleteMethod(client);
			break ;
		case POST:
			this->postMethod(client);
			break ;
		case METH_NUM:
			break ;
	}
}

void	Server::getMethod(Client &client)
{
	if (client.getRequest().getRunScriptBool() == true)//FIXME - forzo per debug
		return ;
	std::cout << "runMethod(): reading file..\n";
	if (client.getRequest().getBodyType() == "text/html")
	{
		std::cout << "Entro nell'override login\n";
		std::getline(this->file, this->resp_body, '\0');
		if (client.getRequest().getCookieKey().empty() == false)
		{
			find_and_replace(this->resp_body, "<!-- <div class=\"client-label\">", "<div class=\"client-label\">");
			find_and_replace(this->resp_body, "</div> -->", "</div>");
			find_and_replace(this->resp_body, "login", client.getCookieData().login);
			find_and_replace(this->resp_body, "<form method=\"GET\" action=\"login/\">", "<!-- <form method=\"GET\" action=\"login/\">");
			find_and_replace(this->resp_body, "</form>", "</form> -->");
		}
		else
			std::cout << "Cookie è vuoto!\n";
		// std::cout << this->resp_body << "\n";
		return ;
	}
	client.sendContentBool() = true;
	client.getBuffer().clear();
	read_file(this->file, client.getBuffer());
	client.getBuffer().push_back('\n');
	client.getBuffer().push_back('\n');
}

void	Server::deleteMethod(Client &client)
{
	if (check_delete(client, this->resp_body, *this, &this->file) != 0)
	{
		std::cout << "protected!" << std::endl;
		return ;
	}
	this->file.close();
	execute_delete(client, this->resp_body, &this->file);
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
	hex_to_char(url);
	std::cout << "FILE: " << url << std::endl;
	if (status_code != 200 || dns == true || autoindex == true)
	{
		if (body.empty() == true)
			body = open_and_read(*file);
	}
	else if (protected_files.find(url) != std::string::npos)
	{
		file->close();
		file->open("www/var/errors/403.html");
		body = open_and_read(*file);
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
		body = open_and_read(*file, "delete_method: cannot open file on success");
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
	body = open_and_read(*file, "delete_method: cannot open file on error");
}

/**
 * @brief Post Method execution: Gets a filename from request <get_filename>, checks if it already exists
 * 
 * creates the file and writes on it time after time going through poll calls <write_on_ofile>
 * 
 * chooses an html page to display according to the operation success or not <choose_html>
 * 
 * @param client > client containing the request
 */
void	Server::postMethod(Client &client)
{	
	Request				&request = client.getRequest();
	std::fstream		html;
	std::string 		file;

	if (request.checkKey("Boundary") == false)
		return ;
	
	file = get_filename(client);
	std::cout << "postMethod(): " << file << std::endl;
	if (open_file(file))
		request.fail(HTTP_CE_CONFLICT, "File already exists!");\

	write_on_ofile(request, file);

	choose_html(*this, client, this->file, html);
	// std::cout << "FAILE: " << file << std::endl;
	this->resp_body = open_and_read(html);
	find_and_replace(this->resp_body, "{MSG}", request.getFailMsg());
}

void	print_bin(std::string filename, char *bin_data, size_t len);

static void	trimBody(Request &request);

int	bodyHeaderParsing(Request &request)
{
	if (request.getBodyHeadersBool())// body gia parsato
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
	std::cout << h_len[0] << std::endl;
	std::getline(request.getRequestStream(), temp, '\n');
	if (temp == "")
		return ;
	else if ("--" + boundary + '\r' == temp)
	{
		headerParsing(request, false);
		//salva la posizione del cursore dopo headerParsing
		h_len[1] = file_cursor_pos(request.getRequestStream());
		std::cout << h_len[1] << std::endl;
	}//else: è un body normale senza immagine, nienlete headerBodyParsing
	else
	{
		h_len[1] = h_len[0];
		if (request.getBodyLen() != (request.getSockBytes() - h_len[0]))
			request.fail(HTTP_CE_METHOD_NOT_ALLOWED);
	}
	bodyHeaderContentLen = request.getSockBytes() - h_len[0];
	bodyContentLen = request.getSockBytes() - h_len[1];
	request.getBytesLeft() -= bodyHeaderContentLen;
	for (size_t i = 0; i != bodyContentLen; i++)
		request.getSockBuff()[i] = request.getSockBuff()[i + h_len[1]];
	request.getBinBody().insert(request.getBinBody().end(), request.getSockBuff(), request.getSockBuff() + bodyContentLen);
	request.getBodyHeadersBool() = true;
}

std::string	get_filename(Client &client)
{
	std::string file;
	std::string val = client.getRequest().getHeader()["Content-Disposition"];
	
	if (val.find("filename=\"") != std::string::npos && val.rbegin()[0] == '"')
	{
		file = val.substr(val.find("filename=\"") + 10, val.find_last_of('\"'));
		if (file.rbegin()[0] == '\"')
			file.erase(file.length() - 1, 1);
	}
	else
		client.getRequest().fail(HTTP_CE_BAD_REQUEST, "Bad \"Content-Disposition\" header format");
	if (client.getLocConf().post_storage.empty() == false)
		file = client.getLocConf().post_storage + file;
	else
		file = client.getSrvConf().post_storage + file;
	return (file);
}

void	write_on_ofile(Request &request, std::string file)
{
	std::ofstream	ofile(file.c_str(), std::ios_base::binary);
	if (ofile.fail())
		std::cout << "Error opening file\n";
	//- Aggiungiamo il numero di caratteri aggiunti dal protocollo al boundary (\r\n--boundary--\r\n)
	size_t	bound_len = request.getHeaderVal("Boundary").length() + 8;
	ofile.write(request.getBinBody().data(), request.getBinBody().size() - bound_len);
}

void	choose_html(Server &srv, Client &client, std::fstream &file, std::fstream &html)
{
	Request &request = client.getRequest();

	if (request.getStatusCode() == 200)
		html.open("www/var/upload/success_upload.html");
	else
		html.open("www/var/errors/fail_upload.html");
	if (file.fail())
	{
		client.getRequest().fail(HTTP_CE_NOT_FOUND, ": html not found!");
		html.open((srv.checkErrorPages(request)).c_str());
	}
}