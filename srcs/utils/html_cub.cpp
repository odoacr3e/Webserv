#include "../../includes/ether.hpp"
#include "../../srcs/hpp/Server.hpp"

std::string		createHtmlCub(t_cgi &cgi_data, Server &srv, Client &client)
{
	std::string		output(client.getBuffer().data(), 100);
	size_t			info_len = 0;

	if (output.compare(0, 3, "OK|") != 0)
	{
		if (cgi_data.pid)
			kill(cgi_data.pid, SIGKILL);
		close(cgi_data.pipe[0]);
		close(cgi_data.pipe[1]);
		srv.getIpPortCgiMap().erase(client.getRequest().getHost());
		return ("");
	}
	info_len = 3;
	if (output.compare(3, 6, "MUSIC-") == 0)
	{
		output = output.substr(9);
		//info_len += 
		std::cout << "new song " << output << "\n";
	}
	(void)info_len;
	return ("");
}
