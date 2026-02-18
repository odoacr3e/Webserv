#include "../../includes/ether.hpp"

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
