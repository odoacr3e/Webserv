
#include "includes/lib.h"

void	crypt_str_from_file(int fd, char *filename)
{
	char 			*gnl = (char *)malloc(sizeof(char) * 1001);
	char			*key = (char *)malloc(sizeof(char) * KEYLEN + 1);
	char			*string = NULL;
	char			*def_file;
	unsigned char 	*ret ;
	int				i = 0;
	int 			j = 0;

	printf("\nCreazione della chiave di criptazione in corso...\n");
	key_generator(key);
	while (find_newline(key) || ft_strlen(key) != 16)
		key_generator(key);
	usleep(500000);
	write(1, ".", 1);
	usleep(500000);
	write(1, ".", 1);
	usleep(500000);
	write(1, ".", 2);
	sleep(1);
	write(1, "\n\n", 2);
	print_format('-', ft_strlen(KEYOK1), 1);
	printf("|");
	print_format(' ', ft_strlen(KEYOK2), 0);
	printf("|\n");
	printf("| %sCHIAVE DI CRIPTAZIONE CREATA CON SUCCESSO%s |\n", T_GREEN, T_WHITE);
	printf("|");
	print_format(' ', ft_strlen(KEYOK2), 0);
	printf("|\n");
	print_format('-', ft_strlen(KEYOK1), 1);
	sleep(1);
	printf("\nCriptazione del file in corso...\n");
	gnl = get_next_line(fd);
	while (gnl)
	{
		string = ft_strappend(string, gnl);
		free(gnl);
		gnl = get_next_line(fd);
	}
	ret = (unsigned char *)malloc(sizeof(unsigned char) * ((ft_strlen(string) * 2) + 1));
	while (string[j] != '\0')
	{
		ret[i] = string[j] + key[i % KEYLEN] + (i / KEYLEN);
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
	def_file = ft_strjoin(filename, "_crypt");
	int	fdret = open(def_file, O_RDWR | O_CREAT);
	write(fdret, ret, ft_strlen((char *)ret));
	close(fdret);
	printf("%sFile criptato con successo (%s)\n", T_GREEN, def_file);
	printf("%s\n", T_WHITE);
	printf("\n");
	printf("\n");
	free(key);
	free(def_file);
	free(gnl);
	free(string);
	free(ret);
}

/* REVIEW - 
** - qualsiasi file nella directory verra criptato
** - la chiave verra inserita nei primi caratteri del file
*/
void	crypt_file(void)
{
	char	*path = malloc(sizeof(char) * 1025);
	char	*defpath;
	int		fd;

	printf("\nInserire il percorso del file da criptare:\n-> ");
	scanf(" %s", path);
	printf("\n");
	if (path[ft_strlen(path) - 1] == '\'' && path[0] == '\'')
		defpath = ft_substr(path, 1, ft_strlen(path) - 2);
	fd = open(defpath, O_RDWR);
	if (fd < 0)
	{
		free(path);
		printf("%s", T_RED);
		print_format('-', ft_strlen("| File non trovato! |"), 1);
		printf("| File non trovato! |\n");
		print_format('-', ft_strlen("| File non trovato! |"), 1);
		printf("%s\n", T_WHITE);
		return ;
	}
	free(path);
	printf("\nInizio della criptazione del file %s%s%s\n\n", T_GREEN, find_filename(defpath), T_WHITE);
	crypt_str_from_file(fd, find_filename(defpath));
	close(fd);
}
