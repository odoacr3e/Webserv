
#include "../../includes/ether.hpp"

//returns 1 if directory is valid, else 0
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

//returns 1 if directory is valid, else 0
bool	valid_file(std::string file)
{
	file = file.substr(file[0] == '/');
	std::ifstream	fd(file.c_str());

	return (fd.fail() == 0);
}
