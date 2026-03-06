#include "../../hpp/Server.hpp"

bool	autoindex_do(Client &client);

/**
 * @brief Creates the response to send to the receiving client according to the method chosen.
 * 
 * - If post is selected it first sends the std::string html page then uses the std::vector<Char> in order to work with binary values.
 * @param client > destination Client
 */
void	Server::processResponse(Client &client)
{
	static int	n_resp;
	std::string msgEndCon(MSG_END_CONNECTION);
	std::vector<char>	&contentData = client.getBuffer();
	std::string	html = createResponse(client);

	if ((client.getPollFd(*this)->events & POLLOUT) == 0)
		return ;
	send(client.getSockFd(), html.c_str(), html.length(), MSG_NOSIGNAL);
	LOG_RESPONSE(html);
	find_and_replace(msgEndCon, "{INDEX}", n_resp++);
	LOG_RESPONSE(msgEndCon);
	if (client.sendContentBool() == true)
		send(client.getSockFd(), contentData.data(), contentData.size(), MSG_NOSIGNAL);
	perror("processResponse(): ");
	client.sendContentBool() = false;
	std::cout << "processResponse() " << client.getRequest().getStatusCode() << " ";
	std::cout <<client.getRequest().getMethod() << "\n";
	client.getRequest().setUrl("");
	client.getRequest().setUrlOriginal("");
	client.getPollFd(*this)->events = POLLIN;
}

/**
 * @brief Creates the html page according to the request demands.
 * 
 * - Returns an autoindex page if a request's url is a valid directory
 * 
 * - Returns an html webpage if reqeust's url is valid path
 * 
 * - Proceeds to execute the chosen method
 * @param client > destination Client
 * @return std::string html page
 */
std::string	Server::createResponse(Client &client)
{
	clearRespVariables();
	checkRequestStatusCode(client);
	assignFileType(client);
	if (autoindex_do(client, client.getRequest().getUrl()))
		createAutoindex(client);
	else
		choose_file(client);
	runMethod(client);
	if (client.getPollFd(*this)->events & POLLOUT)
		return (createHtml(client));
	return ("");
}

/**
 * @brief Chooses which html prefab to open
 * 
 * According to the request type and response type we have existing html prefabs to open and modify
 * 
 * In case of fail, if a custom made error page was provided we use that
 * @param client > destination Client
 */
void	Server::choose_file(Client &client)
{
	std::string	fname;
	std::string	redirect_file;

	if (client.getLocConf().exist && client.getLocConf().ret_text.empty() == false)
		redirect_file = client.getLocConf().ret_text;
	if (client.getRequest().getDnsErrorBool())
		this->file.open("www/var/errors/dns/index.html");
	else if (client.getRequest().getStatusCode() != 200)
	{
		fname = checkErrorPages(client.getRequest());
		this->file.open((fname).c_str());
	}
	else if (redirect_file.empty() == false)
		this->file.open("www/var/default_redirect.html");
	else if (client.getRequest().getRunScriptBool() == false)
	{
		this->file.open(this->resp_url.c_str());
		if (this->file.fail())
		{
			client.getRequest().fail(HTTP_CE_NOT_FOUND, this->resp_url + ": File not found!");
			fname = checkErrorPages(client.getRequest());
			this->file.open((fname).c_str());
		}
	}
}

/**
 * @brief Returns the request's error page if existing, else it returns the server one
 * 
 * @param request > Request to inspect
 * @return std::string Html error page
 */
std::string	Server::checkErrorPages(Request &request)
{
	s_conf_server 	*server = &(*this->_srvnamemap)[request.getHost()];
	s_conf_location	*loc;
	int				status_code = request.getStatusCode();
	std::string 	url = request.getUrl();

	if (server->location.count(url) == 0) // check se non ci sono location
	{
		if (server->err_pages.count(status_code) > 0) // check su server se ci sono error pages adeguate
		{
			this->file.open((server->root + server->err_pages[status_code]).c_str());
			if (this->file.fail() == false)
				return (server->root + server->err_pages[status_code]);
		}
	}
	else if (server->location[url].err_pages.count(status_code) > 0) // controllo se location ha l'error page richiesta
	{
		loc = &server->location[url];
		this->file.open((loc->root + server->location[url].err_pages[status_code]).c_str());
		if (request.getStatusCode() >= 300 && request.getStatusCode() < 400)
			return ("www/var/errors/default_3xx.html");
		else if (this->file.fail() == false)
			return (loc->root + server->location[url].err_pages[status_code]);
		else if (loc->ret_text.empty() == false)
			return ("www/var/default_redirect.html");
		else
			return ("www/var/errors/default.html");
	}
	if (request.getStatusCode() >= 300 && request.getStatusCode() < 400)
		return ("www/var/errors/default_3xx.html");
	else
		return ("www/var/errors/default.html");
}

/**
 * @brief Create a Session Id as a cookie
 * 
 * @return std::string Session id number
 */
std::string createSessionId()
{
	static int id;
	static int s_id_num;

	if (!s_id_num)
		s_id_num = std::rand() % 1000;
	std::string s_id(ft_to_string(s_id_num));
	return (s_id.append(ft_to_string(++id)));
}

/**
 * @brief Creates the html page according to request's data
 * 
 * @param client > Client containing the request
 * @return std::string Html page filled with data
 */
std::string	Server::createHtml(Client &client)
{
	std::ostringstream	response;
	static std::string	http_codes_str[] = VALID_HTTP_STR;
	std::string			url = client.getRequest().getUrl();
	int 				status = client.getRequest().getStatusCode();
	bool				gen_cookie = client.getLocConf().gen_cookie;
	bool				cookie_empty = client.getRequest().getCookieKey().empty();

	response << "HTTP/1.1 "
	         << status << " "
	         << http_codes_str[checkValidHttpCode(status)] << "\r\n";
	response << "Content-Type: " << client.getRequest().getBodyType() << "\r\n";
	response << "Cache-Control: no-cache, no-store, must-revalidate" << "\r\n";
	response << "Pragma: no-cache" << "\r\n";
	response << "Expires: 0" << "\r\n";
	response << "Connection: close" << "\r\n";
	if (client.getLocConf().exist && gen_cookie && cookie_empty)
	{
		std::cout << "set-Cookie:\n";
		client.getRequest().getCookieKey() = createSessionId();
		response << "Set-Cookie: session_id=" << client.getRequest().getCookieKey() << "; Path=/; HttpOnly\r\n";
		this->_cookie_map[client.getRequest().getCookieKey()].login = "client " + client.getRequest().getCookieKey();
	}
	if (client.sendContentBool() == true)
		response << "Content-Length: " << client.getBuffer().size() << "\r\n\r\n";
	else
	{
		response << "Content-Length: " << this->resp_body.size() << "\r\n\r\n";
		response << this->resp_body << "\n\n";
	}
	std::cout << "createHtml() URL: " << url << std::endl;
	return (response.str());
}

/**
 * @brief This function fills the error page with the current request's informations
 * 
 * - Error codes, error messages ecc
 * 
 * @param client > Client containing the request
 * @param html > Return page html
 */
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

/**
 * @brief Reads the file type from te request and updates the file type in the server class variable type
 * 
 * @param client > Client containing the request
 */
void	Server::assignFileType(Client &client)
{
	this->resp_url = client.getRequest().getUrl();
	size_t last_dot = this->resp_url.find_last_of('.');
	std::string url;

	if (last_dot != std::string::npos)
	{
		url = this->resp_url.substr(last_dot);
		if (url != ".html" && url != ".css")
			this->type = "image/";
		this->type += url.erase(0, 1);
	}
}

/**
 * @brief utility function to clear the variables used to create an html response in server class
 * 
 * - std::string	resp_body
 * 
 * - std::string	resp_url
 * 
 * - std::string	type
 * 
 * - fstream		file
 */
void	Server::clearRespVariables()
{
	this->resp_body.clear();
	this->resp_url.clear();
	this->type.clear();
	this->type = "text/";
	if (this->file.is_open())
		this->file.close();
	this->file.clear();
}

/**
 * @brief Helper function: reads request's status code, method, and autoindex bool from location
 * 
 * @param client 
 * @return true if autoindex is to be done
 * @return false if autoindex is not to be done
 */
bool	autoindex_do(Client &client, std::string url)
{
	e_http_codes	status_code = client.getRequest().getStatusCode();
	e_methods		method = client.getRequest().getMethodEnum();
	bool			autoindex_bool = client.getRequest().getAutoIndexBool();

	if (status_code == 200 && autoindex_bool && valid_directory(url) && method != POST)
		return (true);
	else
		return (false);
}

/**
 * @brief Helper function: reads request's status code and sets fail accordingly
 * 
 * @param client > Client containing the request
 */
void	checkRequestStatusCode(Client &client)
{
	std::string	ret_text = client.getLocConf().ret_text;
	bool		loc_exists = client.getLocConf().exist;
	int			ret_code = client.getLocConf().ret_code;

	if (loc_exists && ret_code != 0 && ret_code != 200)
		client.getRequest().fail(ret_code, ret_text);
}