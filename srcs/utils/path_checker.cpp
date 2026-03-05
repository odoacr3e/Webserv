#include "../../includes/ether.hpp"

//returns true if directory is valid, else false
bool	valid_directory(std::string directory)
{
	DIR		*dir_fd;
	bool	valid = true;

	if (directory == "/")
		return (valid);
	directory = directory.substr(directory[0] == '/');
	dir_fd = opendir(directory.c_str());
	valid = (dir_fd != NULL);
	return (closedir(dir_fd), valid);
}

//returns true if directory is valid, else false
bool	valid_file(std::string file)
{
	file = file.substr(file[0] == '/');
	std::ifstream	fd(file.c_str());

	return (fd.fail() == 0);
}

bool	valid_exec(std::string file)
{
	std::ifstream	fd(file.c_str());

	return (fd.fail() == 0);
}
