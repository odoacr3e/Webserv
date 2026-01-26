
#include "../../includes/ether.hpp"
#include "../hpp/Request.hpp"

typedef struct s_conf_server	t_conf_server;
typedef struct s_conf_location	t_conf_location;

//SECTION - app root/alias to a file

std::string	app_root_alias(std::string file, std::string root, std::string alias, \
 std::string replace)
{
	std::cout << "append_root_alias" << std::endl;
	std::cout << "BEFORE " << file << std::endl;

	if (alias.empty() == false)
	{
		if (alias[0] == '/')
			alias.erase(0, 1);
		std::cout << "replace " << replace << " with " << alias << std::endl;
		if (file.find(replace) != std::string::npos)//find /srcs/
			file.replace(0, replace.length(), alias);
	}
	else if (root.empty() == false)
	{
		if (root[0] == '/')
			root.erase(0, 1);
		file = root + file;
	}
	else
		std::cout << "no root or alias\n";
	std::cout << "AFTER " << file << std::endl;
	return (file);
}

//append root or alias to a file
//this overload appends SERVER root
std::string	app_root_alias(std::string file, t_conf_server &srv)
{
	file = app_root_alias(file, srv.root, "", "");
	return (file);
}

//append root or alias to a file
//this overload appends: 
// if (srv.location.count(loc) == OK) --> append LOCATION alias/root
// else append SERVER root
std::string	app_root_alias(std::string file, t_conf_server &srv, std::string loc)
{
	if (srv.location.count(loc) != 0)
		file = app_root_alias(file, srv.location[loc].root, srv.location[loc].alias, loc);
	else
		file = app_root_alias(file, srv.root, "", "");
	return (file);
}

//append root or alias to a file
//this overload appends LOCATION alias/root
std::string	app_root_alias(std::string file, t_conf_location &loc)
{
	file = app_root_alias(file, loc.root, loc.alias, loc.path);
	return (file);
}
