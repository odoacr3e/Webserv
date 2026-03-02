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

//returns bytes read
int	read_cgi(Client &client, s_cgi &cgi)
{
	std::string	output;
	int			bytes;

	if (cgi.cgiHeaderParsing == false)
	{
		client.getBuffer().resize(CGI_HEADER_LEN);
		bytes = read(cgi.pipe[0], client.getBuffer().data() + cgi.bytes_read, CGI_HEADER_LEN - cgi.bytes_read);
		if (bytes <= 0)
			return (std::cerr << "readFastcgiError: cgi is gone\n", -1);
		cgi.bytes_read += bytes;
		if (cgi.bytes_read != 14)
			return (bytes);
		client.getBuffer().push_back('\0');
		output = client.getBuffer().data();
		client.getBuffer().clear();
		cgi.bytes_read = 0;
		if (output.compare(0, 3, "OK|") != 0 || output.rbegin()[0] != '|')
			return (std::cerr << "readFastcgiErrorFormat: " << output << "\n", -1);
		bytes = std::atoi(output.c_str() + 3);
		if (bytes <= 0)
			return (std::cerr << "readFastcgiErrorSize: " << output << "\n", -1);
		cgi.output_len = bytes;
		cgi.cgiHeaderParsing = true;
		return (1);
	}
	client.getBuffer().resize(cgi.bytes_read + CHUNK_READ);
	bytes = read(cgi.pipe[0], client.getBuffer().data() + cgi.bytes_read, CHUNK_READ);
	if (bytes <= 0)
		return (-1);
	cgi.bytes_read += bytes;
	client.getBuffer().resize(cgi.bytes_read);
	std::cout << "read_cgi(): bytes to read " << bytes << "\n";
	std::cout << "read_cgi(): bytes read " << client.getBuffer().size() << "\n";
	return (bytes);
}
