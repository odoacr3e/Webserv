#include "../../includes/ether.hpp"
#include "../../srcs/hpp/Server.hpp"

std::string		createHtmlCub(t_cgi &cgi_data, Server &srv, Client &client)
{
	std::string		output(client.getBuffer().data(), 100);
	size_t			sep_index;

	if (output.compare(0, 3, "OK|") != 0)
	{
		if (cgi_data.pid)
			kill(cgi_data.pid, SIGKILL);
		close(cgi_data.pipe[0]);
		close(cgi_data.pipe[1]);
		srv.getIpPortCgiMap().erase(client.getRequest().getHost());
		return ("");
	}
	if (output.compare(3, 6, "MUSIC-") == 0)
	{
		sep_index = output.find_first_of('-', 3) + 1;
		std::cout << "new song " << output.substr(sep_index) << "\n";
	}
	client.sendContentBool() = true;
	client.getBuffer().erase(client.getBuffer().begin(), client.getBuffer().begin() + 64);
	return ("");
}
