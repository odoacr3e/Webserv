#include "../../hpp/Server.hpp"

//void	manage_cookies();

void	Server::processResponse(Client &client)
{
	static int	n_resp;
	std::string msgEndCon(MSG_END_CONNECTION);
	std::vector<char>	&contentData = client.getBuffer();
	std::string	html = createResponse(client);

	if ((client.getPollFd()->events & POLLOUT) == 0)
		return ;
	send(client.getSockFd(), html.c_str(), html.length(), MSG_NOSIGNAL);
	LOG_RESPONSE(html);
	find_and_replace(msgEndCon, "{INDEX}", n_resp++);
	LOG_RESPONSE(msgEndCon);
	if (client.sendContentBool() == true)
		send(client.getSockFd(), contentData.data(), contentData.size(), MSG_NOSIGNAL);
	client.sendContentBool() = false;
	std::cout << "processResponse() " << client.getRequest().getStatusCode() << " ";
	std::cout <<client.getRequest().getMethod() << "\n";
	client.getRequest().setUrl("");
	client.getRequest().setUrlOriginal("");
	client.getPollFd()->events = POLLIN;
}

std::string	Server::createResponse(Client &client) // create html va messo anche percorso per il file
{
	std::fstream	file;
	std::string		body;
	std::string		type("text/");
	std::string		url;

	if (client.getLocConf().exist && client.getLocConf().ret_code != 0)
		client.getRequest().fail(client.getLocConf().ret_code, client.getLocConf().ret_text);
	url = client.getRequest().getUrl();
	if (url.find_last_of('.') != std::string::npos)
	{
		if (url.substr(url.find_last_of('.')) != ".html" && url.substr(url.find_last_of('.')) != ".css")
			type = "image/";
		type += url.substr(url.find_last_of('.')).erase(0, 1);
	}
	else
		type += "html";
	if (client.getRequest().getStatusCode() == 200 && client.getRequest().getAutoIndexBool() && valid_directory(url) && client.getRequest().getMethodEnum() != POST)
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
			if (file.fail() == false)
				return (server->root + server->err_pages[status_code]);
		}
	}
	else if (server->location[url].err_pages.count(status_code) > 0) // controllo se location ha l'error page richiesta
	{
		loc = &server->location[url];
		file.open((loc->root + server->location[url].err_pages[status_code]).c_str());
		if (file.fail() == false)
			return (loc->root + server->location[url].err_pages[status_code]);
		if (request.getStatusCode() >= 300 && request.getStatusCode() < 400)
			return ("www/var/errors/default_3xx.html");
		else
			return ("www/var/errors/default.html");
	}
	if (request.getStatusCode() >= 300 && request.getStatusCode() < 400)
		return ("www/var/errors/default_3xx.html");
	else
		return ("www/var/errors/default.html");
}

//  cookie = seed|i
std::string createSessionId()
{
	static int id;
	static int s_id_num;

	if (!s_id_num)
		s_id_num = std::rand() % 1000;
	std::string s_id(ft_to_string(s_id_num));
	return (s_id.append(ft_to_string(++id)));
}

// NOTE - crea html come body per la risposta da inviare al client
std::string	Server::createHtml(Client &client, std::string &body)
{
	std::ostringstream	response;
	static std::string	http_codes_str[] = VALID_HTTP_STR;
	std::string			url = client.getRequest().getUrl();
	int 				status = client.getRequest().getStatusCode();

	response << "HTTP/1.1 "
	         << status << " "
	         << http_codes_str[checkValidCode(status)] << "\r\n";
	response << "Content-Type: " << client.getRequest().getBodyType() << "\r\n";
	response << "Cache-Control: no-cache, no-store, must-revalidate" << "\r\n";
	response << "Pragma: no-cache" << "\r\n";
	response << "Expires: 0" << "\r\n";
	if (client.getLocConf().exist && client.getLocConf().gen_cookie && \
		client.getRequest().getCookieKey().empty() == true)
	{
		std::cout << "set-Cookie:\n";
		client.getRequest().getCookieKey() = createSessionId();
		response << "Set-Cookie: session_id=" << client.getRequest().getCookieKey() << "; Path=/; HttpOnly\r\n";
		this->_cookie_map[client.getRequest().getCookieKey()].login = "client " + client.getRequest().getCookieKey();
	}
	if (client.sendContentBool() == true)
	{
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

void	fill_error_page(Client &client, std::string &html)
{
	e_http_codes	code;

	while (find_and_replace(html, "{CODE}", client.getRequest().getStatusCode()));
	while (find_and_replace(html, "{TITLE}", client.getRequest().getFailMsg()));
	while (find_and_replace(html, "{MSG}", client.getRequest().getFailMsg()));
	code = client.getRequest().getStatusCode(); 
	if (code >= 300 && code <= 399)
		find_and_replace(html, "{MSG_TYPE}", "New_url");
	else
		find_and_replace(html, "{MSG_TYPE}", "Detail");
	find_and_replace(html, "{DETAIL}", client.getLocConf().ret_text);
	find_and_replace(html, "{SOUND_HOME}", "ET telefono casa");
	find_and_replace(html, "{SOUND_RETRY}", "Ritenta, sfigato");
	find_and_replace(html, "{URL}", client.getLocConf().ret_text);
}
