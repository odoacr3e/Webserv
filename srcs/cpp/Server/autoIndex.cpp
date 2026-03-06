#include "../../hpp/Server.hpp"
#define SIZE_STR "B", "KB", "MB", "GB", "TB"

static std::string	calculateSize(size_t s);

/**
 * @brief Creates the autoindex page
 * 
 * @param client > Client containing the request
 */
void	Server::createAutoindex(Client &client)
{
	std::ifstream	file;
	std::string		line;
	dirent			*content;
	std::string		url;

	if (client.getAllowedMethods() & MASK_DELETE)
		file.open("www/var/autoindex/delete_autoindex.html");
	else
		file.open("www/var/autoindex/autoindex.html");
	url = client.getRequest().getUrl();
	while (std::getline(file, line))
	{
		line.push_back('\n');
		find_and_replace(line, "{PATH}", client.getRequest().getUrlOriginal());
		this->resp_body += line;
		if (line.find("<tbody") != std::string::npos)
			break ;
	}
	content = findUrlDirectory(url);
	while (content)
	{
		std::string dname = content->d_name;
		if (dname[0] != '.')
			listDirectoriesAutoIndex(client, url, content);
		content = findUrlDirectory(url);
	}
	while (std::getline(file, line))
		if (line.find("</tbody>") != std::string::npos)
			break ;
	this->resp_body += line;
	LOG_TERM << "createAutoindex()\n";
	LOG_TERM << client.getRequest().getUrlOriginal() << "\n";
	std::getline(file, line, '\0');
	find_and_replace(line, "/{URL}/", client.getRequest().getUrlOriginal());
	find_and_replace(line, "{SERVER_NAME}", "3 UOMINI E 1 WEBSERVER");
	this->resp_body += line;
	if (client.getRequest().getCookieKey().empty() == false)
	{
		find_and_replace(this->resp_body, "<!-- <div class=\"client-label\">", "<div class=\"client-label\">");
		find_and_replace(this->resp_body, "</div> -->", "</div>");
		find_and_replace(this->resp_body, "login", client.getCookieData().login);
		find_and_replace(this->resp_body, "<form method=\"GET\" action=\"login/\">", "<!-- <form method=\"GET\" action=\"login/\">");
		find_and_replace(this->resp_body, "</form>", "</form> -->");
	}
}


/**
 * @brief Fills the html prefab with directories data aout the autoindexed folder
 * 
 * @param client > Client containing the request
 * @param url > Url of the current folder
 * @param cont > Variable needed by the stat function
 */
void Server::listDirectoriesAutoIndex(Client &client, std::string &url, dirent *cont)
{
	std::ifstream var;
	std::string	path;
	std::string line;
	std::string	s_cont;
	struct stat	info;

	s_cont = (std::string)cont->d_name + (cont->d_type == 8 ? "" : "/");
	std::memset(&info, 0, sizeof(struct stat));
	path = url + '/' + cont->d_name;
	stat(path.c_str(), &info);
	if (client.getAllowedMethods() & MASK_DELETE)
	{
		var.open("www/var/autoindex/delete_var.html");
	}
	else
		var.open("www/var/autoindex/var.html");
	if (var.fail())
	{
		LOG_TERM << "Could not open html file!\n";
		return ;
	}
	while(std::getline(var, line))
	{
		line.append("\n");
		find_and_replace(line, "href=\"", "href=\"" + s_cont);
		find_and_replace(line, "{NAME}", s_cont);
		find_and_replace(line, "{SIZE}", calculateSize(info.st_size));
		find_and_replace(line, "{MODIFY}", std::ctime(&info.st_mtim.tv_sec));
		this->resp_body += line;
	}
}

/**
 * @brief this function calculates the size of the autoindexed folder
 * 
 * @param s size returned by stat
 * @return std::string 
 */
static std::string	calculateSize(size_t s)
{
	int 	len = 0;
	size_t	tmp = s;
	static std::string	size_str[] = {SIZE_STR, ""}; 

	while (tmp > 0)
	{
		tmp /= 10;
		len++;
	}
	std::string type = size_str[(len - 1) / 3];
	while (len > 3)
	{
		s /= 1000;
		len -= 3;
	}
	return (ft_to_string(s) + " " + type);
}
