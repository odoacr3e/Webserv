#include "pokedex.h"

static char	**get_info();
static void	image_id(char **data, char **info, t_str *buff);

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

int	error_cgi(char *error)
{
	write(1, "KO|00000000-1|", 14);
	write(1, error, ft_strlen(error));
	return (0);
}

int	main(int ac, char **av)
{
	char	**data;
	char	**info;

	if (ac == 1)
		return (error_cgi("Please give a valid move name\n"));
	info = get_info();
	STR(move, av[1]);
	STR(buff, "");
	move.m->str_lower(&move);
	if (daft_init("www/cgi-bin/pokedex/media", "SETTINGS.md") != 0)
		daft_init("media", "SETTINGS.md");
	daft_swap(MOVES);
	data = daft_get(move.buff);
	if (!data || !info)
	{
		free_matrix(info);
		str_printf(&buff, "move %s not found.\n", move.buff);
		error_cgi(buff.buff);
		FREE(move.m);
		return (str_terminate(), daft_quit(), 1);
	}
	image_id(data, info, &buff);
	for (int i = 1; data[i]; i++)
	{
		str_printf(&buff, "|%s:%s", info[i], data[i]);
	}
	print_cgi(buff.buff, buff.len);
	FREE(move.m);
	return (str_terminate(), free_matrix(info), daft_quit(), 0);
}

static char	**get_info()
{
	t_fd	fd;
	char	*line;
	char	**info;

	fd = openfd("www/cgi-bin/pokedex/media/data/metadata_pokemon_moves.csv", "r");
	if (!fd.p)
		fd = openfd("media/data/metadata_pokemon_moves.csv", "r");
	if (!fd.p)
		return (err_printf("moves.cgi error: missing files\n"), NULL);
	line = gnl();
	closefd(fd);
	if (!line)
		return (err_printf("moves.cgi error: empty data\n"), NULL);
	info = ft_split(line, ',');
	FREE(line);
	return (info);
}

static void	image_id(char **data, char **info, t_str *buff)
{
	int	i;

	for (i = 0; info[i]; i++)
	{
		if (ft_strncmp("type", info[i], INT_MAX) == 0)
			break ;
	}
	if (!info[i])
		return (str_printf(buff, "image not found"), (void)0);
	str_printf(buff, "{%d}%s", ID_MOVE, data[i]);
}
