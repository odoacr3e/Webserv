
#include "includes/lib.h"

int	main(int ac, char **av) 
{
	write(2, "entro CRYPTER\n", 14);
	if (ac != 3)
		return (write(1, "Error format\n", ft_strlen("Error format\n")));
	if (atoi(av[1]) == 1)
		crypt_str(av[2]);
	else if (atoi(av[1]) == 2)
	{
		printf("ENTRO\n");
		decrypt_str(av[2]);
	}
	else
		return (write(2, "esco CRYPTER else\n", 18));
}
