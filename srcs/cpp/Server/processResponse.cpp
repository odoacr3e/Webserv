#include "../../hpp/Server.hpp"

std::string	createHtml(Client &client, const std::string &body);

void	Server::processResponse(std::vector<pollfd>::iterator &it)
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
		// if (url.rbegin()[0] != '/')
			// std::cout << "passed a file in choose_file with no extension!" << std::endl;
	}
	if (client.getRequest().getAutoIndexBool() && valid_directory(url) && client.getRequest().getMethodEnum() != POST)
		createAutoindex(client, body);
	//else if (client.getRequest().getMethodEnum() != POST)
	else
		choose_file(client, file, url);
	client.getRequest().setBodyType(type);
	client.getBuffer().clear();
	runMethod(client, body, file);
	return (createHtml(client, body));
}


// NOTE - crea html come body per la risposta da inviare al client
std::string	createHtml(Client &client, const std::string &body)
{
	std::ostringstream	response;
	std::string			http_codes_str[] = VALID_HTTP_STR;
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
