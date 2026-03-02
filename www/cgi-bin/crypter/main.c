
#include "includes/lib.h"

int	print_cgi(char *output, int output_len)
{
	char	*str;
	int		len;
	int		padding_zero;

	write(1, "OK|", 3);
	str = ft_itoa(output_len);
	if (!str)
		return (1);
	len = ft_strlen(str);
	padding_zero = 10 - len;
	if (padding_zero > 0)
		write(1, "00000000000", padding_zero);
	write(1, str, len);
	write(1, "|", 1);
	write(1, output, output_len);
	free(str);
	return (0);
}

int	main(int ac, char **av)
{	
	srand(time(NULL));
	write(2, "CRYPTER: entro CRYPTER\n", 24);
	if (ac != 3)
		return (write(1, "Error format\n", ft_strlen("Error format\n")));
	if (strcmp((av[ac - 1]), "crypt_string") == 0)
	{
		// write(2, "CRYPTER: Critto la stringa\n", 28);
		crypt_str(av[1]);
	}
	else if (strcmp(av[ac - 1], "decrypt_string") == 0)
	{
		// write(2, "CRYPTER: Decripto la stringa\n", 30);
		decrypt_str(av[1]);
	}
	else
		return (0/* write(2, "CRYPTER: esco CRYPTER else\n", 28 */);
	// write(2, "CRYPTER: esco dal cripter\n", 27);
}
