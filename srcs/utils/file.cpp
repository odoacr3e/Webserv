#include "../../includes/ether.hpp"
#include "../../srcs/hpp/Server.hpp"
#include "../../srcs/hpp/Client.hpp"

void	close_fd(int *fd)
{
	if (*fd > 0)
		close(*fd);
	*fd = 0;
}

int read_file(std::fstream &stream, std::vector<char> &vect)
{
	static char		buf[2048];
	std::streamsize	bytes;

	if (stream.fail())
	{
		std::cerr << "Errore: " << std::strerror(errno) << std::endl;
		return (1);
	}
	while (stream)
	{
		stream.read(buf, sizeof(buf));
		bytes = stream.gcount();
		if (bytes > 0)
			vect.insert(vect.end(), buf, buf + bytes);
	}
	return (0);
}

int read_file(std::ifstream &stream, std::vector<char> &vect)
{
	static char		buf[2048];
	std::streamsize	bytes;

	if (stream.fail())
	{
		std::cerr << "Errore: " << std::strerror(errno) << std::endl;
		return (1);
	}
	while (stream)
	{
		stream.read(buf, sizeof(buf));
		bytes = stream.gcount();
		if (bytes > 0)
			vect.insert(vect.end(), buf, buf + bytes);
	}
	vect.rbegin()[0] = 0;
	return (0);
}

int read_file(std::string name, std::vector<char> &vect)
{
	std::ifstream stream(name.c_str(), std::ios_base::binary);

	return (read_file(stream, vect));
}

int read_file(std::string name, std::vector<char> &vect, int bytes)
{
	std::ifstream stream(name.c_str(), std::ios_base::binary);

	vect.resize(bytes);
	stream.read(vect.data(), bytes);
	vect.resize(stream.gcount());
	return (0);
}

//returns 1 on error, 0 on success
int	read_fastcgi(Client &client, s_cgi &cgi)
{
	std::string	fd_name;
	std::string	output;
	int			bytes;

	client.getBuffer().resize(FASTCGI_HEADER_LEN);
	bytes = read(cgi.pipe[0], client.getBuffer().data(), FASTCGI_HEADER_LEN);
	if (bytes <= 0)
		return (std::cerr << "readFastcgiError: cgi is gone\n", 1);
	client.getBuffer().push_back('\0');
	output = client.getBuffer().data();
	if (output.compare(0, 3, "OK|") != 0 || output.rbegin()[0] != '|')
		return (std::cerr << "readFastcgiErrorFormat: " << output << "\n", 1);
	bytes = std::atoi(output.c_str() + 3);
	if (bytes <= 0)
		return (std::cerr << "readFastcgiErrorSize: " << output << "\n", 1);
	cgi.output_len = bytes;
	client.getBuffer().resize(bytes);
	int	bytes_total = bytes;
	bytes = 0;
	bytes += read(cgi.pipe[0], client.getBuffer().data() + bytes, bytes_total);
	while (bytes != bytes_total)
		bytes += read(cgi.pipe[0], client.getBuffer().data() + bytes, bytes_total);
	std::cout << "read_fastcgi(): bytes to read " << bytes << "\n";
	std::cout << "read_fastcgi(): bytes read " << client.getBuffer().size() << "\n";
	return (0);
}
