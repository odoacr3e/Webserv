#include "../../hpp/Conf.hpp"

//Conf print overload
std::ostream &operator<<(std::ostream &os, Conf &c)
{
	// os << "####################################\n";
	// os << "\033[35mPrint of all configurations:\n";
	// os << "\033[33m{MAIN BLOCK}\n";
	// if (c.getMainUser().empty() == false)
	// 	os << "\033[34mUser:\t\033[33m" << c.getMainUser() << "\n";
	// os << "\033[33m{SERVER BLOCK}";
	// os << c.getConfServer();
	// os << "\033[33m{IP:PORT / SERVER_CONF MAP}";
	// os << c.getSrvNameMap();
	// os << "\033[0m";
	(void)c;
	return (os);
}

void	print_err_pages(std::ostream &stream, std::map<int, std::string> err_pages, std::string format)
{
	stream << format << "\033[0m\033[1;35merror_pages -> \t\033[3;37m";
	for (std::map<int, std::string>::iterator it = err_pages.begin(); it != err_pages.end(); ++it)
	{
		if (it != err_pages.begin())
			stream << "\t\t\t";
		stream << (*it).first << ", " << (*it).second;
		if (it != --err_pages.end())
			stream << std::endl;
	}
}

//t_conf_server print overload
std::ostream &operator<<(std::ostream &os, t_conf_server &srv)
{
	// os << "\033[0m{\n";
	// os << "\033[0m\033[1;35m    root ->\t\t\033[3;37m" << srv.root << std::endl;
	// os << "\033[0m\033[1;35m    index ->\t\t\033[3;37m" << srv.index << std::endl;
	// os << "\033[0m\033[1;35m    client_max_body ->\033[3;37m\t" << srv.client_max_body_size << std::endl;
	// os << "\033[0m\033[1;35m    autoindex -> \033[37m\t" << (srv.autoindex ? "true" : "false") << std::endl;
	// os << "\033[0m\033[1;35m    server names ->\033[3;37m";
	// os << srv.server_names;
	// print_err_pages(os, srv.err_pages, "\n    ");
	// os << "\033[0m";
	// print_map(os, srv.location, "\033[0m\033[1;93m    location\033[3;37m", "\033[1;95m", NULL);
	// os << "\033[0m}\033[0m\n";
	(void)srv;
	return (os);
}

//t_conf_location print overload
std::ostream &operator<<(std::ostream &os, t_conf_location &loc)
{
	// os << "\n\t\033[1;95mconf root: \033[37m\t\t" << loc.conf_root;
	// os << "\n\t\033[1;95mreal root: \033[37m\t\t" << loc.root;
	// os << "\n\t\033[1;95mfastcgi_param: \033[37m" << loc.cgiparam;
	// if (loc.run_script)
	// 	os << "\n\t\033[1;95mscript_type: \033[37m" << loc.script_type;
	// os << "\n\t\033[1;95mret code: \033[37m\t" << loc.ret_code;
	// os << "\n\t\033[1;95mret text: \033[37m\t" << loc.ret_text;
	// os << "\n\t\033[1;95mret uri: \033[37m\t" << loc.ret_uri;
	// os << "\n\t\033[1;95mautoindex: \033[37m\t" << (loc.autoindex ? "true" : "false") << std::endl;
	// print_err_pages(os, loc.err_pages, "\t");
	(void)loc;
	return (os);
}

//SrvNameMap print overload
std::ostream &operator<<(std::ostream &os, SrvNameMap &map)
{
	os << "\n\033[34mIp Address list: \033[0m" << std::endl;
	print_map(map);
	return (os);
}
