#include "../tester.hpp"
#define SPACE " \f\n\r\t\v\b"

void	erase_space(std::string &s)
{
	size_t	i;
	size_t	j;

	i = s.find_first_of(SPACE);
	while (i != s.npos)
	{
		j = s.find_first_not_of(SPACE, i);
		if (j == s.npos)
			j = s.length();
		j -= i;
		s.erase(i, j);
		i = s.find_first_of(SPACE, i);
	}
}

void	erase_ansi(std::string &s)
{
	size_t	i;
	size_t	j;

	i = s.find_first_of("\033");
	while (i != s.npos)
	{
		j = s.find_first_of('m', i);
		if (j == s.npos)
			j = s.length();
		else
			j += 1;
		j -= i;
		s.erase(i, j);
		i = s.find_first_of("\033[", i);
	}
}
