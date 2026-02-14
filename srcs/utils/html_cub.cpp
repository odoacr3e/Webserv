#include "../../includes/ether.hpp"
#include "../../srcs/hpp/Server.hpp"

std::string		createHtmlCub(t_cgi &cgi_data, Server &srv, Client &client)
{
	std::string		new_song;

	if (cgi_data.output.compare(0, 3, "OK|") != 0)
	{
		if (cgi_data.pid)
			kill(cgi_data.pid, SIGKILL);
		close(cgi_data.pipe[0]);
		close(cgi_data.pipe[1]);
		srv.getIpPortCgiMap().erase(client.getRequest().getHost());
	}
	else if (cgi_data.output.compare(3, 6, "MUSIC-") == 0)
	{
		new_song = cgi_data.output.substr(9);
		std::cout << "new song " << new_song << "\n";
	}
	return ("");
}
