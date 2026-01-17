#include "../../hpp/Conf.hpp"

//Conf print overload
std::ostream &operator<<(std::ostream &os, Conf &c)
{
	os << "####################################\n";
	os << "\033[35mPrint of all configurations:\n";
	os << "\033[33m{MAIN BLOCK}\n";
	if (c.getMainUser().empty() == false)
		os << "\033[34mUser:\t\033[33m" << c.getMainUser() << "\n";
	os << "\033[33m{SERVER BLOCK}";
	os << c.getConfServer();
	os << "\033[33m{IP:PORT / SERVER_CONF MAP}";
	os << c.getSrvNameMap();
	os << "\033[0m";
	return (os);
}

//t_conf_server print overload
std::ostream &operator<<(std::ostream &os, t_conf_server &srv)
{
	std::cout << "\033[0m{\n";
	std::cout << "\033[0m\033[1;35m    root ->\t\t\033[3;37m" << srv.root << std::endl;
	std::cout << "\033[0m\033[1;35m    index ->\t\t\033[3;37m" << srv.index << std::endl;
	std::cout << "\033[0m\033[1;35m    client_max_body ->\033[3;37m\t" << srv.client_max_body_size << std::endl;
	std::cout << "\033[0m\033[1;35m    server names ->\033[3;37m";
	std::cout << srv.server_names;
	os << "\033[0m";
	print_map(os, srv.location, "\033[0m\033[1;93m    location\033[3;37m", "\033[1;95m", NULL);
	std::cout << "\033[0m}\033[0m\n";
	return (os);
}

//t_conf_location print overload
std::ostream &operator<<(std::ostream &os, t_conf_location &loc)
{
	os << "\n\t\033[1;95malias: \033[37m\t\t" << loc.alias;
	os << "\n\t\033[1;95mroot: \033[37m\t\t" << loc.root;
	os << "\n\t\033[1;95mfastcgi_param: \033[37m" << loc.cgiparam;
	os << "\n\t\033[1;95mret code: \033[37m\t" << loc.ret_code;
	os << "\n\t\033[1;95mret text: \033[37m\t" << loc.ret_text;
	os << "\n\t\033[1;95mret uri: \033[37m\t" << loc.ret_uri;
	return (os);
}

//SrvNameMap print overload
std::ostream &operator<<(std::ostream &os, SrvNameMap &map)
{
	os << "\n\033[34mIp Address list: \033[0m" << std::endl;
	print_map(map);
	return (os);
}
