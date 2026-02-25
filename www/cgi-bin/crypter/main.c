
#include "includes/lib.h"

int	main(int ac, char **av)
{
	// return (write(1, "ciao", 4), write(1, "\0", 1));
	return (0);
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
