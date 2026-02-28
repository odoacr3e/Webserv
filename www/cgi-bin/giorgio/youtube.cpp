#include "../../../includes/ether.hpp"

void	vect_split(std::vector<std::string> &vect, std::string s, char c);

/*
//https://www.youtube.com/results?search_query=gatti+arrabbiati

//https://www.youtube.com/results?search_query=gatti
*/
int main(int ac, char **av)
{
	if (ac != 2 || av == NULL || av[1] == NULL || av[1][0] == '\0')
	{
		return (write(1, "Error\0", 6));
	}
	std::string argv(av[1]);
	while (std::isspace(argv[0]) != 0)
		argv.erase(0, 1);
	if (argv.empty() == true)
		return (write(1, " \0", 2));
	std::vector<std::string> split;
	std::string new_string("https://www.youtube.com/results?search_query=");
	vect_split(split, argv, ' ');
	for (size_t i = 0; i < split.size(); i++)
	{
		if (split[i].empty() == false && i != 0)
			new_string += '+';
		new_string += split[i];
	}
	write(1, new_string.c_str(), new_string.length());
	write(1, "\0", 1);
	std::cout << "\n";
}

void	vect_split(std::vector<std::string> &vect, std::string s, char c)
{
	std::string	temp;
	size_t		i;

	while (s.empty() == false && s[0] != c)
	{
		i = s.find(c);
		if (i == std::string::npos)
		{
			vect.push_back(s);
			return ;
		}
		temp = s.substr(0, i);
		s.erase(0, temp.length());
		s.erase(0, s[0] == c);
		vect.push_back(temp);
	}
}
