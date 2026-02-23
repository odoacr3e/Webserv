
#include "includes/lib.h"

void	key_generator(char *str)
{
	int 	i = 0;
	char	*keypool = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-+=*/;:<>.,$#@";
	int		keypoollen = 78;

	while (i < KEYLEN)
	{
		str[i] = keypool[rand() % keypoollen];
		i++;
	}
	str[i] = 0;
	if (ft_strlen(str) != 16)
		key_generator(str);
}

void	crypt_str(char *str)
{
	char	*key = (char *)malloc(sizeof(char) * KEYLEN + 1);
	int		i = 0;
	int 	j = 0;

	key_generator(key);
	while (find_newline(key) || ft_strlen(key) != 16)
		key_generator(key);
	unsigned char 	*ret = (unsigned char *)malloc(sizeof(unsigned char) * ((ft_strlen(str) * 2) + 1));
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
	write(1, key, ft_strlen(key));
	write(1, ret, ft_strlen((char *)ret));
	free(key);
	free(ret);
}
