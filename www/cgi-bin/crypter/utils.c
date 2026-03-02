
#include "includes/lib.h"


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

char	*ft_strappend(char *s1, char *s2)
{
	char	*ret;
	char	*tmp;

	ret = malloc(sizeof(char) * (ft_strlen(s1) * ft_strlen(s2) + 1));
	tmp = ft_strdup(s1);
	free(s1);
	ret = ft_strjoin(tmp, s2);
	return (ret);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*str;

	str = (char *)malloc(sizeof(*s) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (i >= start && j < len)
		{
			str[j] = s[i];
			j++;
		}
		i++;
	}
	str[j] = 0;
	return (str);
}

char	*find_filename(char *path)
{
	int	i = ft_strlen(path) - 1;
	while (i > 0 && path[i] != '\\' && path[i] != '/')
		i--;
	return (&path[i + 1]);
}

//NOTE - copy s1 in s2 (without checking if s1 len = s2 len)
void	ft_copystr(const char s1[1024], char *s2)
{
	int i = 0;
	while (s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
}

unsigned char *crypt_name(char *str, char *key)
{
	unsigned char 	*ret = (unsigned char *)malloc(sizeof(unsigned char) * ((ft_strlen(str) * 2) + 1));
	int	j = 0;
	int	i = 0;
	while (str[j] != '\0')
	{
		ret[i] = str[j] + key[i % KEYLEN] + (i / KEYLEN);
		ret[i + 1] = '0';
		if (ret[i] > 127)
		{
			ret[i] -= 127;
			ret[i + 1] += '4' - '0';
		}
		if (ret[i] > 0 && ret[i] <= 32)
		{
			ret[i] += 33;
			ret[i + 1] += '1' - '0';
		}  
		else if (ret[i] == 127)
		{
			ret[i] -= 33;
			ret[i + 1] += '2' - '0';
		}
		i += 2;
		j++;
	}
	ret[i] = '\0';
	return (ret);
}

unsigned char *ft_unstrjoin(unsigned char *s1, unsigned char *s2)
{
	char *ret = (char *)malloc(sizeof(char) * (ft_strlen((char *)s1) + ft_strlen((char *)s2) + 1));
	int i = 0;
	int j = 0;
	while (s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		ret[i] = s2[j];
		j++;
	}
	ret[i] = '\0';
	return ((unsigned char *)ret);
}

int ft_strlenfinal(char *str)
{
	int i = 0;
	int cont = 0;

	while (str[i] && str[i + 1])
	{
		cont++;
		if (str[i] == ' ')
			i++;
		else
			i += 2;
	}
	return (cont);
}

int	find_newline(char *str)
{
	int	i = 0;

	while (str[i] && str[i] != '\n')
		i++;
	return (str[i] == '\n');
}

//NOTE - in newline: 1 is yes, 0 is no
void	print_format(char c, int len, int newline)
{
	int	i = 0;

	while (i < len)
	{
		printf("%c", c);
		i++;
	}
	if (newline == 1)
		printf("\n");
}
