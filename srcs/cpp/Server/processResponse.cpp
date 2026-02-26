#include "../../hpp/Server.hpp"

std::string	createHtml(Client &client, const std::string &body);

void	Server::processResponse(Client &client)
{
	static int	n_resp;
	std::string msgEndCon(MSG_END_CONNECTION);
	std::vector<char>	&contentData = client.getBuffer();
	std::string	html = createResponse(client);

	if ((client.getPollFd()->events & POLLOUT) == 0)
		return ;
	send(client.getSockFd(), html.c_str(), html.length(), 0);
	LOG_RESPONSE(html);
	find_and_replace(msgEndCon, "{INDEX}", n_resp++);
	LOG_RESPONSE(msgEndCon);
	if (client.sendContentBool() == true)
		send(client.getSockFd(), contentData.data(), contentData.size(), 0);
	client.sendContentBool() = false;
	std::cout << "processResponse() " << client.getRequest().getStatusCode() << " ";
	std::cout <<client.getRequest().getMethod() << "\n";
	client.getRequest().setUrl("");
	client.getRequest().setUrlOriginal("");
	client.getPollFd()->events = POLLIN;
}

// NOTE - crea la risposta html da inviare al client tramite HTTP
std::string	Server::createResponse(Client &client) // create html va messo anche percorso per il file
{
	std::fstream	file;
	std::string		body;
	std::string		type("text/");
	std::string		url;

	url = client.getRequest().getUrl();
	if (url.find_last_of('.') != std::string::npos)
	{
		if (url.substr(url.find_last_of('.')) != ".html" && url.substr(url.find_last_of('.')) != ".css")
			type = "image/";
		type += url.substr(url.find_last_of('.')).erase(0, 1);
	}
	else
	{
		type += "html";
	}
	if (client.getRequest().getAutoIndexBool() && valid_directory(url) && client.getRequest().getMethodEnum() != POST)
		createAutoindex(client, body);
	else
		choose_file(client, file, url);
	client.getRequest().setBodyType(type);
	runMethod(client, body, file);
	if (client.getPollFd()->events & POLLOUT)
		return (createHtml(client, body));
	return ("");
}

void	Server::choose_file(Client &client, std::fstream &file, std::string url)
{
	std::string	fname;

	if (client.getRequest().getDnsErrorBool())
		file.open("www/var/errors/dns/index.html");
	else if (client.getRequest().getStatusCode() != 200)
	{
		fname = checkErrorPages(client.getRequest());
		file.open((fname).c_str());
	}
	else if (client.getRequest().getRunScriptBool() == false)
	{
		file.open(url.c_str());
		if (file.fail())
		{
			client.getRequest().fail(HTTP_CE_NOT_FOUND, url + ": File not found!");
			fname = checkErrorPages(client.getRequest());
			file.open((fname).c_str());
		}
	}
}

//FIXME - da inserire append root/alias in config file
std::string	Server::checkErrorPages(Request &request)
{
	std::ifstream	file;
	s_conf_server 	*server = &(*this->_srvnamemap)[request.getHost()];
	s_conf_location	*loc;
	int				status_code = request.getStatusCode();
	std::string 	url = request.getUrl();

	if (server->location.count(url) == 0) // check se non ci sono location
	{
		if (server->err_pages.count(status_code) > 0) // check su server se ci sono error pages adeguate
		{
			file.open((server->root + server->err_pages[status_code]).c_str());
			if (file.fail() == true)
				return (url_rooting("/errors/default.html", *server));
			return (url_rooting(server->err_pages[status_code], *server));
		}
	}
	else if (server->location[url].err_pages.count(status_code) > 0) // controllo se location ha l'error page richiesta
	{
		loc = &server->location[url];
		file.open((server->root + server->location[url].err_pages[status_code]).c_str());
		std::cout << "Location prova ad aprire: " << loc->root + server->location[url].err_pages[status_code] << std::endl;
		if (file.fail() == true)
			return (url_rooting("/errors/default.html", *loc));
		return (url_rooting(server->location[url].err_pages[status_code], *loc));
	}
	return (server->root + "/errors/default.html"); // return di default
}

// NOTE - crea html come body per la risposta da inviare al client
std::string	createHtml(Client &client, const std::string &body)
{
	std::ostringstream	response;
	static std::string	http_codes_str[] = VALID_HTTP_STR;
	std::string			url = client.getRequest().getUrl();
	int 				status = client.getRequest().getStatusCode();

	response << "HTTP/1.1 "
	         << status << " "
	         << http_codes_str[checkValidCode(status)] << "\r\n";
	response << "Content-Type: " << client.getRequest().getBodyType() << "\r\n";
	if (client.sendContentBool() == true)
	{
		response << "Cache-Control: no-cache, no-store, must-revalidate" << "\r\n";
		response << "Pragma: no-cache" << "\r\n";
		response << "Expires: 0" << "\r\n";
		response << "Content-Length: " << client.getBuffer().size();
		response << "\r\n\r\n";
	}
	else
	{
		response << "Content-Length: " << body.size() << "\r\n\r\n";
		response << body << "\n\n";
	}
	std::cout << "createHtml() URL: " << url << std::endl;
	return (response.str());
}
