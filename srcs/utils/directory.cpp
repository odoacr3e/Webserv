#include "../../includes/ether.hpp"

// NOTE - cerca directory dentro l'URL passato come parametro, viene chiamata come gnl e ad ogni ritorno ritorna la cartella successiva
dirent	*findUrlDirectory(std::string url)
{
	static DIR	*dir;
	dirent		*content;

	if (url.empty())
	{
		if (dir)
			closedir(dir);
		dir = NULL;
		return (NULL);
	}
	if (!dir)
	{
		dir = opendir(url.c_str());
		if (!dir)
			return (NULL);
	}
	content = readdir(dir);
	if (!content)
	{
		closedir(dir);
		dir = NULL;
		return (NULL);
	}
	// std::cout << "findUrlDirectory(): " << content->d_name << std::endl;
	return (content);
}

dirent	*findUrlDirectory(std::string url, DIR *dir)
{
	dirent		*content;

	if (!dir)
	{
		dir = opendir(url.c_str());
		if (!dir)
			return (NULL);
	}
	content = readdir(dir);
	return (content);
}


/// @brief equivalent to shell rm dir/*
/// @param dir_path path of the directory
/// @return 1 on errors, 0 on success
int		eraseDirectory(std::string dir_path)
{
	dirent	*content;

	content = findUrlDirectory(dir_path);
	if (!content)
		return (1);
	if (dir_path.back() != '/')
		dir_path.push_back('/');
	while (content != NULL)
	{
		if (content->d_name[0] != '.')
			std::remove((std::string(dir_path + content->d_name)).c_str());
		content = findUrlDirectory(dir_path);
	}
	return (0);
}

void	ft_ls(std::string path, std::string &files)
{
	DIR		*dir;
	dirent	*content;

	if (path.rbegin()[0] != '/')
		path.append("/");
	dir = opendir(path.c_str());
	if (!dir)
		return ;
	content = findUrlDirectory(path, dir);
	while (content)
	{
		if (content->d_type == DT_DIR)
		{
			if (content->d_name[0] != '.')
				ft_ls(path + content->d_name, files);
		}
		else
			files += (path + content->d_name + '|');
		content = findUrlDirectory(path, dir);
	}
	closedir(dir);
}
