#include "../../../includes/ether.hpp"
#include "../../../includes/status_codes.hpp"

/*
	trim a str until s[i] == c, then return 0.
	if \0 is found, str is set to "", 1 is returned.
	if c is \0, 1 is returned.

	c = ' '
	s = "   ciao" ---> "ciao"
*/
bool	trim_equal_left(std::string &s, char c)
{
	size_t	i;

	if (!c)
		return (1);
	i = s.find_first_not_of(c);
	if (i == std::string::npos)
		return (1);
	s.erase(0, i);
	return (0);
}

/*
	trim a str until s[i] != c, then return 0.
	if \0 is found, str is set to "", 1 is returned.
	if c is \0, 1 is returned.

	c = ' '
	s = "ciao come stai" ---> " come stai"
*/
bool	trim_diff_left(std::string &s, char c)
{
	size_t	i;

	if (!c)
		return (1);
	i = s.find_first_of(c);
	if (i == std::string::npos)
		return (1);
	s.erase(0, i);
	return (0);
}

/*
	trim a str FROM RIGHT until s[i] == c, then return 0.
	if \0 is found, str is set to "", 1 is returned.
	if c is \0, 1 is returned.

	c = ' '
	s = "ciao     " ---> "ciao"
*/
bool	trim_equal_right(std::string &s, char c)
{
	size_t	start;
	size_t	end;

	if (!c)
		return (1);
	start = s.find_first_of(c);
	if (start == std::string::npos)
		return (1);
	start = s.find_first_not_of(c);
	if (start == std::string::npos)
		return (1);
	std::cout << s.substr(start) << "\n";
	end = s.substr(start).length();
	end = start + end;
	s.erase(start, end);
	return (0);
}

/*
	trim a str FROM RIGHT until s[i] != c, then return 0.
	if \0 is found, str is set to "", 1 is returned.
	if c is \0, 1 is returned.

	c = ' '
	s = "ciao come" ---> "ciao "
*/
bool	trim_diff_right(std::string &s, char c)
{
	size_t	start;
	size_t	end;

	if (!c)
		return (1);
	start = s.find_first_not_of(c);
	if (start == std::string::npos)
		return (1);
	start = s.find_first_of(c);
	if (start == std::string::npos)
		return (1);
	std::cout << s.substr(start) << "\n";
	end = s.substr(start).length();
	end = start + end;
	s.erase(start, end);
	return (0);
}
