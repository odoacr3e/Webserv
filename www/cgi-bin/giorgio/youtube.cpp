#include "../../../includes/include.hpp"

void	vect_split(std::vector<std::string> &vect, std::string s, char c);
char    *ft_itoa(int num);

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

static int	numlen(int n)
{
	int	i;

	i = 0;
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static void	min_int(char *s)
{
	s[1] = '2';
	s[2] = '1';
	s[3] = '4';
	s[4] = '7';
	s[5] = '4';
	s[6] = '8';
	s[7] = '3';
	s[8] = '6';
	s[9] = '4';
	s[10] = '8';
	s[11] = '\0';
}

static void	check_number(char *s, int n, int j)
{
	char	tmp[12];
	int		i;

	i = 0;
	if (n < 0)
	{
		s[0] = '-';
		if (n == -2147483648)
		{
			min_int(s);
			return ;
		}
		n *= -1;
		i = 1;
	}
	while (n > 0)
	{
		tmp[j++] = (n % 10) + 48;
		n /= 10;
	}
	tmp[j--] = '\0';
	while (tmp[j])
		s[i++] = tmp[j--];
	s[i] = '\0';
}

char	*ft_itoa(int n)
{
	char	*s;

	if (n > 0)
		s = (char *)malloc(sizeof(char) * (numlen(n) + 1));
	else if (n < 0)
		s = (char *)malloc(sizeof(char) * (numlen(-n) + 2));
	else
	{
		s = (char *)malloc(sizeof(char) * 2);
		s[0] = '0';
		s[1] = '\0';
		return (s);
	}
	check_number(s, n, 0);
	return (s);
}

int	print_cgi(char *output, int output_len)
{
	char	*str;
	int		len;
	int		padding_zero;

	write(1, "OK|", 3);
	str = ft_itoa(output_len);
	if (!str)
		return (1);
	len = std::strlen(str);
	padding_zero = 10 - len;
	if (padding_zero > 0)
		write(1, "00000000000", padding_zero);
	write(1, str, len);
	write(1, "|", 1);
	write(1, output, output_len);
	free(str);
	return (0);
}

int	error_cgi(std::string error)
{
	write(1, "KO|00000000-1|", 14);
	write(1, error.c_str(), error.length());
	return (0);
}

/*
//https://www.youtube.com/results?search_query=gatti+arrabbiati

//https://www.youtube.com/results?search_query=gatti
*/
int main(int ac, char **av)
{
	if (ac != 2 || av == NULL || av[1] == NULL || av[1][0] == '\0')
	{
		return (error_cgi("error\n"));
	}
	std::string argv(av[1]);
	while (std::isspace(argv[0]) != 0)
		argv.erase(0, 1);
	if (argv.empty() == true)
		return (error_cgi("error\n"));
	std::vector<std::string> split;
	std::string new_string("https://www.youtube.com/results?search_query=");
	vect_split(split, argv, ' ');
	for (size_t i = 0; i < split.size(); i++)
	{
		if (split[i].empty() == false && i != 0)
			new_string += '+';
		new_string += split[i];
	}
	print_cgi((char*)(void*)new_string.c_str(), (int)new_string.length());
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
