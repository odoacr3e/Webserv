#include "../../../includes/ether.hpp"

template <typename T>
std::string	ft_to_string(T data)
{
	std::ostringstream	stream;

	stream << data;
	return (stream.str());
}

//returns true if replace succeed, else false
template <typename old_s, typename new_s>
bool	find_and_replace(std::string &str, old_s to_replace, new_s content)
{
	std::string	temp_replace;
	std::string	temp_content;

	temp_replace = ft_to_string(to_replace);
	temp_content = ft_to_string(content);
	if (str.find(temp_replace) != std::string::npos)
		str.replace(str.find(temp_replace), temp_replace.size(), temp_content);
	else
		return (false);
	return (true);
}

template <typename old_s>
bool	find_and_erase(std::string &str, old_s to_replace)
{
	std::string	temp_replace;

	temp_replace = ft_to_string(to_replace);
	if (str.find(temp_replace) != std::string::npos)
		str.replace(str.find(temp_replace), temp_replace.size(), 0);
	else
		return (false);
	return (true);
}
