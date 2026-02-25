
#include "includes/lib.h"

void	copy_str(char *input, char *str)
{
	int	i = 0;

	while (input[i])
	{
		str[i] = input[i];
		i++;
	}
	str[i] = '\0';
}

void	copy_key(char *input, char *key)
{
	int	i = 0;

	while (i < KEYLEN)
	{
		key[i] = input[i];
		i++;
	}
	key[i] = '\0';
}

void	decrypt_str(char *str)
{
	char 	*key = (char *)malloc(sizeof(char) * 17);
	int 	i = 0;
	int 	j = KEYLEN;

	copy_key(str, key);// key: ore+L*mpap:5#-Ho
	printf("%d\n", ft_strlen((char *) str));
	printf("key: %s\n", key);
	while (str[j])// ore+L*mpap:5#-HoS4O4.4]4#5(44574U4V4
	{
		str[j] = str[j] - key[(j - KEYLEN) % KEYLEN] - ((j - KEYLEN) / KEYLEN);
		if (str[j + 1] == '1')
			str[j] -= 33;
		if (str[j + 1] == '2')
			str[j] += 33;
		if (str[j + 1] == '4')
			str[j] += 127;
		if (str[j + 1] == '5')
		{
			str[j] -= 33;
			str[j] += 127;
		}
		else if (str[j + 1] == '6')
		{
			str[j] += 33;
			str[j] += 127;
		}
		j += 2;
	}
	char *ret = (char *)malloc(sizeof(char) * ft_strlenfinal((char *)str) + 1);
	j = 0;
	while (str[i + KEYLEN] && str[i + KEYLEN + 1])
	{
		ret[j] = (char)str[i + KEYLEN];
		i += 2;
		j++;
	}
	ret[j] = '\0';
	write(1, ret, ft_strlen(ret));
	write(1, "\0", 1);
	free(ret);
	free(key);
}
