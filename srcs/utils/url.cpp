
#include "../../includes/ether.hpp"
#include "../hpp/Request.hpp"

typedef struct s_conf_server	t_conf_server;
typedef struct s_conf_location	t_conf_location;

//SECTION - app root/alias to a file

//append root to an url from t_conf_server
std::string	url_rooting(std::string url, t_conf_server &srv)
{
	std::string		dir;
	std::string		dir_original;
	std::string		file;

	dir = url;
	file = url;
	trim_from(dir, dir.find_last_of('/') + 1);//+1 skip /
	normalize_url(&dir);
	if (dir == "/")
		dir.clear();
	dir = srv.root + dir;
	file.erase(0, file.find_last_of('/') + 1);//+1 includes /
	return (dir + file);
}

//append root or alias to an url from t_conf_location
std::string	url_rooting(std::string url, t_conf_location &loc)
{
	std::string		dir;
	std::string		file;

	dir = url;
	file = url;
	trim_from(dir, dir.find_last_of('/') + 1);//+1 skip /
	normalize_url(&dir);
	if (dir == "/")
		dir.clear();
	else if (loc.alias == true)
		dir.erase(0, loc.conf_root.length());
	dir = loc.root + dir;
	file.erase(0, file.find_last_of('/') + 1);//+1 includes /
	return (dir + file);
}

//SECTION - normalize_url

//normalize an url in this form: /dir/dir/file/
//@url: address of url
//----------------------
//@ret: void
void	normalize_url(std::string *url)
{
	if ((*url)[0] == '/')
		(*url).erase(0, 1);
	if ((*url).rbegin()[0] != '/')
		(*url).push_back('/');
}

//normalize an url in this form: /dir/dir/file/
//@url: copy of url
//----------------------
//@ret: new url
std::string	normalize_url(std::string url)
{
	normalize_url(&url);
	return (url);
}
