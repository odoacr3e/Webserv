#include "../../includes/ether.hpp"

//returns 1 if directory is valid, else 0
bool	valid_directory(std::string directory)
{
	DIR		*dir_fd;
	bool	valid;

	dir_fd = opendir(directory.c_str());
	valid = (dir_fd != NULL);
	closedir(dir_fd);
	return (valid);
}

//returns 1 if directory is valid, else 0
bool	valid_file(std::string file)
{
	std::ifstream	fd(file.c_str());

	return (fd.fail() == 0);
}
