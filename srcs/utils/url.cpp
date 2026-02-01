
#include "../../includes/ether.hpp"
#include "../hpp/Request.hpp"

typedef struct s_conf_server	t_conf_server;
typedef struct s_conf_location	t_conf_location;

//SECTION - app root/alias to a file

//append root to an url from t_conf_server
std::string	url_rooting(std::string url, t_conf_server &srv)
{
	std::string		file;
	std::string		dir;

	file = url_arg_get(url, '/');
	dir = url_arg_remove(url, '/');
	url_normalize(&dir);
	if (dir == "/")
		dir.clear();
	dir = srv.root + dir;
	file.erase(0, file.find_last_of('/') + 1);//+1 includes /
	return (dir + file);
}

//append root or alias to an url from t_conf_location
std::string	url_rooting(std::string url, t_conf_location &loc)
{
	std::string		file;
	std::string		dir;

	file = url_arg_get(url, '/');
	dir = url_arg_remove(url, '/');
	url_normalize(&dir);
	if (dir == "/")
		dir.clear();
	else if (loc.alias == true)
		dir.erase(0, loc.conf_root.length());
	dir = loc.root + dir;
	return (dir + file);
}

//SECTION - url_normalize

//normalize an url in this form: /dir/dir/file/
//@url: address of url
//----------------------
//@ret: void
void	url_normalize(std::string *url)
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
std::string	url_normalize(std::string url)
{
	url_normalize(&url);
	return (url);
}

// removes url argument
/*
	INPUT						LIMITER			RETURNS
	*-------------------------------------------------------------------*
	|/www/var/					|	/	--->	/www/var/				|
	|/www/var/index.html		|	/	--->	/www/var/				|
	|/script/crypt?text			|	?	--->	/script/crypt?			|
	|/script/crypt?value=text	|	=	--->	/script/crypt?value=	|
	|/script/crypt/text			|	/	--->	/script/crypt/			|
	*-------------------------------------------------------------------*
*/
std::string	url_arg_remove(std::string url, char limiter)
{
	size_t	pos;

	pos = url.find_last_of(limiter);
	if (pos == std::string::npos)
		return (url);
	pos += 1;
	return (url.erase(pos, url.length() - pos));
}

// returns url argument
/*
	INPUT						LIMITER			RETURNS
	*-------------------------------------------------------------------*
	|/www/var/					|	/	--->							|
	|/www/var/index.html		|	/	--->	index.html				|
	|/script/crypt?text			|	?	--->	text					|
	|/script/crypt?value=text	|	=	--->	text					|
	|/script/crypt/text			|	/	--->	text					|
	*-------------------------------------------------------------------*
*/
std::string	url_arg_get(std::string url, char limiter)
{
	size_t	pos;

	pos = url.find_last_of(limiter);
	if (pos == std::string::npos)
		return ("");
	return (url.erase(0, pos + 1));
}
